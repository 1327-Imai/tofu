#include "Texture.h"

class DX12base {
public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	ComPtr<ID3D12Device> GetDevice();
};

class Model {
public:
	D3D12_RESOURCE_DESC GetResDesc();
};

void Texture::LoadTexture(const wchar_t* fileName) {

	HRESULT result;

	if (fileName) {
		//WIC�e�N�X�`���̃��[�h
		result = LoadFromWICFile(
			fileName ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}
	else {
		//WIC�e�N�X�`���̃��[�h
		result = LoadFromWICFile(
			L"Resources/white1x1.png" ,
			WIC_FLAGS_NONE ,
			&metadata ,
			scratchImg
		);
	}

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages() ,
		scratchImg.GetImageCount() ,
		scratchImg.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//��
	textureResouceDesc.Height = (UINT)metadata.height;	//����
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = dx12base->GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//�S�̈�փR�s�[
			img->pixels ,			//���f�[�^�A�h���X
			(UINT)img->rowPitch ,	//1���C���T�C�Y
			(UINT)img->slicePitch	//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}
}

void Texture::CreateSRV() {
	HRESULT result;

	//�f�X�N���v�^�q�[�v�̐���
	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dx12base->GetDevice()->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�n���h��
	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	srvDesc.Format = model->GetResDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = model->GetResDesc().MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12base->GetDevice()->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);
}


void Texture::Draw() {
	//SRV�q�[�v�̐ݒ�R�}���h
	dx12base->GetCmdList()->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	dx12base->GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);
}

//�A�N�Z�b�T
void Texture::SetDx12Base(DX12base* dx12base) {
	this->dx12base = dx12base;
}

void Texture::SetModel(Model* model) {
	this->model = model;
}