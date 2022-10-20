#include <random>
#include <DirectXTex.h>

#include "DX12base.h"
#include "Model.h"
#include "WorldTransform.h"
#include "GameObject3D.h"

#include "MathFunc.h"

#include "WinApp.h"
#include "input.h"
#include "ViewProjection.h"

WinApp winApp_;

const double PI = 3.141592;

//�����V�[�h������
std::random_device seed_gen;
//�����Z���k�E�c�C�X�^�[�̗����G���W��
std::mt19937_64 engine(seed_gen());
//�����͈͂̎w��
std::uniform_real_distribution<float> distPosX(-100.0 , 100.0);
std::uniform_real_distribution<float> distPosY(-50.0 , 50.0);
std::uniform_real_distribution<float> distPosZ(30.0 , 60.0);
std::uniform_real_distribution<float> distRot(0 , XMConvertToRadians(360.0f));

#pragma region//�\����

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	Vector4 color; //�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	Matrix4 mat; //3D�ϊ��s��
};

struct Object3D {

	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�s���O(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//���[���h�ϊ�
	WorldTransform worldTransform;

};

#pragma endregion

#pragma region//�֐��̃v���g�^�C�v�錾
//�E�B���h�E�v���V�[�V��
LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

//3D�I�u�W�F�N�g������
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device);

void UpdateObject3d(Object3D* object , ViewProjection viewProjection , XMMATRIX& matProjection);

void DrawObject3d(Object3D* object , DX12base dx12base , Model model);

void MoveObject3d(GameObject3D* object , BYTE key[256]);

void SetRandomPositionObject3d(Object3D* object);
void SetRandomRotateObject3d(Object3D* object);

#pragma endregion//�֐��̃v���g�^�C�v�錾

//main�֐�
int WINAPI WinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPSTR , _In_ int) {

	//WindowsAPI����������
	winApp_.Initialize();
	Input* input_ = new Input;

#pragma endregion//�E�B���h�E�̐���

#pragma region//���b�Z�[�W���[�v

	HRESULT result;

	DX12base dx12base;
	dx12base.SetWinApp(&winApp_);
	dx12base.Initialize();

#pragma region// �L�[�{�[�h�f�o�C�X�̐���
	// DirectInput�̏�����
	input_->DirectInputInit(winApp_);

	input_->DirectInputCreate(winApp_);
#pragma endregion


#pragma region//�`�揉��������

#pragma region//���_�f�[�^

	Model model;
	model.SetDx12Base(&dx12base);
	model.LoadModel("Resources/triangle.obj");
	model.Initialize();

#pragma endregion

#pragma region//���_�V�F�[�_�[
	//���_�V�F�[�_�[�t�@�C���̓ǂݍ��݂ƃR���p�C��
	ID3DBlob* vsBlob = nullptr;		//���_�V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;	//�G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl" ,									//�V�F�[�_�[�t�@�C����
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//�C���N���[�h�\�ɂ���
		"main" ,											//�G���g���[�|�C���g��
		"vs_5_0" ,											//�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//�f�o�b�O�p�ݒ�
		0 ,
		&vsBlob ,
		&errorBlob
	);

	//�V�F�[�_�[�R�[�h�̃G���[
	//�G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region//�s�N�Z���V�F�[�_�[
	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl" ,									//�V�F�[�_�[�t�@�C����
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//�C���N���[�h�\�ɂ���
		"main" ,												//�G���g���[�|�C���g��
		"ps_5_0" ,											//�V�F�[�_�[���f���ݒ�
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//�f�o�b�O�p�ݒ�
		0 ,
		&psBlob ,
		&errorBlob
	);

	//�V�F�[�_�[�R�[�h�̃G���[
	//�G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz���W
			"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//�@���x�N�g��
			"NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//uv���W
			"TEXCODE" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
	};

#pragma region//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;			//�w�ʉ������O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;			//�|���S�����h��Ԃ�
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;	//���C���[�t���[���\��
	pipelineDesc.RasterizerState.DepthClipEnable = true;					//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//���ʐݒ�(�A���t�@�l)
	blenddesc.BlendEnable = false;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��

	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;								//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//�[�x�l�t�H�[�}�b�g

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;								//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;								//1�s�N�Z���ɂ�1��̃T���v�����O
#pragma endregion

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region//���[�g�p�����[�^
	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[���猩����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
#pragma endregion

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma region//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;					//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc ,
		D3D_ROOT_SIGNATURE_VERSION_1_0 ,
		&rootSigBlob ,
		&errorBlob);
	assert(SUCCEEDED(result));

	result = dx12base.GetDevice()->CreateRootSignature(
		0 ,
		rootSigBlob->GetBufferPointer() ,
		rootSigBlob->GetBufferSize() ,
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();
#pragma endregion

	//�p�C�v���C���X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dx12base.GetDevice()->CreateGraphicsPipelineState(&pipelineDesc , IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma region//�萔�o�b�t�@

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0 , nullptr , (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = Vector4(1.0f , 1.0f , 1.0f , 0.5f);

	//�萔�o�b�t�@�̐���
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

#pragma endregion

#pragma region//�ˉe�ϊ�
	//�P�ʍs�����
	//constMapTransform->mat = XMMatrixIdentity();

	////���s���e�ϊ��s��̌v�Z
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
	//	0.0f , WINDOW_WIDTH ,
	//	WINDOW_HEIGHT , 0.0f ,
	//	0.0f , 1.0f
	//);

	//�������e�ϊ��s��̌v�Z
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp_.window_width / winApp_.window_height ,
		0.1f , 1000.0f
	);

#pragma endregion

#pragma region//�r���[�ϊ��s��
	ViewProjection viewProjection_;
	viewProjection_.Initialize();
	viewProjection_.UpdateView();

	float angle = 0.0f;

#pragma endregion

#pragma region//���[���h�ϊ��s��

	//3D�I�u�W�F�N�g�̐�
	const size_t kObjectCount = 50;

	Object3D object3ds[kObjectCount];

	GameObject3D gameObject;
	GameObject3D gameObject2;

	for (int i = 0; i < _countof(object3ds); i++) {

		//������
		InitializeObject3d(&object3ds[i] , dx12base.GetDevice());
		object3ds[i].worldTransform.initialize();

		//�������牺�͐e�q�\���̃T���v��
		//�擪�ȊO�Ȃ�
		if (i > 0) {
			////�ЂƂO�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
			//object3ds[i].worldTransform.parent = &object3ds[i - 1].worldTransform;
			////�e�I�u�W�F�N�g��9���̑傫��
			//object3ds[i].worldTransform.scale = {0.9f , 0.9f , 0.9f};
			////�e�I�u�W�F�N�g�ɑ΂���Z���܂���30�x��]
			//object3ds[i].worldTransform.rotation = {0.0f , 0.0f , XMConvertToRadians(30.0f)};

			////�e�I�u�W�F�N�g�ɑ΂���Z��������-8.0���炷
			//object3ds[i].worldTransform.translation = {0.0f , 0.0f , -8.0f};

			//�����_���ɔz�u����
			SetRandomPositionObject3d(&object3ds[i]);
			SetRandomRotateObject3d(&object3ds[i]);
		}

		//object3ds->Update();
	}

	gameObject.SetDX12Base(&dx12base);
	gameObject.Initialize();
	gameObject.SetViewProjection(&viewProjection_);
	gameObject.SetMatProjection(&matProjection);

	gameObject2.SetDX12Base(&dx12base);
	gameObject2.Initialize();
	gameObject2.SetViewProjection(&viewProjection_);
	gameObject2.SetMatProjection(&matProjection);

#pragma endregion

#pragma region//�e�N�X�`��
	//�摜�C���[�W�f�[�^�̍쐬
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/texture.jpg" ,
		WIC_FLAGS_NONE ,
		&metadata ,
		scratchImg
	);

	//�摜�C���[�W�f�[�^�̍쐬
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/reimu.png" ,
		WIC_FLAGS_NONE ,
		&metadata2 ,
		scratchImg2
	);


	ScratchImage mipChain{};
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

	ScratchImage mipChain2{};
	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg2.GetImages() ,
		scratchImg2.GetImageCount() ,
		scratchImg2.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain2
	);
	if (SUCCEEDED(result)) {
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata2.format = MakeSRGB(metadata2.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//��
	textureResouceDesc.Height = (UINT)metadata.height;	//����
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc2{};
	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc2.Format = metadata2.format;
	textureResouceDesc2.Width = metadata2.width;	//��
	textureResouceDesc2.Height = (UINT)metadata2.height;	//����
	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResouceDesc2.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = dx12base.GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff2 = nullptr;
	result = dx12base.GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc2 ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff2)
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

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata2.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img2 = scratchImg2.GetImage(i , 0 , 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff2->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//�S�̈�փR�s�[
			img2->pixels ,			//���f�[�^�A�h���X
			(UINT)img2->rowPitch ,	//1���C���T�C�Y
			(UINT)img2->slicePitch	//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

#pragma endregion

#pragma region//�V�F�[�_�[���\�[�X�r���[
	//�f�X�N���v�^�q�[�v�̐���
	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = dx12base.GetDevice()->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�n���h��
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = model.GetResDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = model.GetResDesc().MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12base.GetDevice()->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);

	//�񖇖�
	//SRV�q�[�v�̐擪�n���h�����擾
	UINT incremantSize = dx12base.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incremantSize;

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};	//�ݒ�\����
	srvDesc2.Format = textureResouceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx12base.GetDevice()->CreateShaderResourceView(texBuff2.Get() , &srvDesc2 , srvHandle);

#pragma endregion

#pragma endregion//�`�揉��������

#pragma region//�Q�[�����[�v
	while (true) {

#pragma region//�E�B���h�E���b�Z�[�W����
		//���b�Z�[�W������?
		if (PeekMessage(&winApp_.msg , nullptr , 0 , 0 , PM_REMOVE)) {
			TranslateMessage(&winApp_.msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&winApp_.msg);	//�v���V�[�V���Ƀ��b�Z�[�W�𑗂�
		}

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (winApp_.msg.message == WM_QUIT) {
			break;
		}
#pragma endregion//�E�B���h�E���b�Z�[�W����

#pragma region//DirectX���t���[������

		// �L�[�{�[�h���̎擾�J�n
		input_->Update();

#pragma region//�X�V����

		//�r���[�ϊ�
		//�����ꂩ�̃L�[�������Ă�����
		if (input_->PushKey(DIK_D) || input_->PushKey(DIK_A)) {

			//�������L�[�ɉ�����angle�𑝌�������
			if (input_->PushKey(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			if (input_->PushKey(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			//angle���W�A������Y������ɉ�]�B���a��-100
			viewProjection_.eye.x = -100 * sinf(angle);
			viewProjection_.eye.z = -100 * cosf(angle);

			//�r���[�ϊ��s�����蒼��
			viewProjection_.UpdateView();

		}

		//MoveObject3d(&object3ds[0] , input_->key);

		for (int i = 0; i < _countof(object3ds); i++) {
			UpdateObject3d(&object3ds[i] , viewProjection_ , matProjection);
		}

		MoveObject3d(&gameObject , input_->key);

		gameObject.Update();
		gameObject2.Update();

#pragma endregion//�X�V����

		//�`��O����
		dx12base.PreDraw();

#pragma region//�`�揈��

		//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx12base.GetCmdList()->SetPipelineState(pipelineState.Get());
		dx12base.GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());

		//�v���~�e�B�u�`��̐ݒ�R�}���h
		dx12base.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//model.Draw();

		//���_�o�b�t�@�\�r���[���Z�b�g����R�}���h
		dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

		//SRV�q�[�v�̐ݒ�R�}���h
		dx12base.GetCmdList()->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

		//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		if (input_->PushKey(DIK_SPACE)) {
			//�񖇖ڂ��w�������悤�ɍ�������
			srvGpuHandle.ptr += incremantSize;
		}

		//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		dx12base.GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);

		for (int i = 0; i < _countof(object3ds); i++) {
			//DrawObject3d(&object3ds[i] , dx12base , model);
		}

		gameObject.Draw();
		gameObject2.Draw();

#pragma endregion

		//�`��㏈��
		dx12base.PostDraw();

#pragma endregion//DirectX���t���[������

	}
#pragma endregion//�Q�[�����[�v

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(device.Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	//�E�B���h�E�N���X�o�^����
	UnregisterClass(winApp_.w.lpszClassName , winApp_.w.hInstance);
#pragma endregion//���b�Z�[�W���[�v

	return 0;
}


#pragma region//�֐��̒�`

//3D�I�u�W�F�N�g������
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device) {

	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̐���
	result = device.Get()->CreateCommittedResource(
		&cbHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&object->constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = object->constBuffTransform->Map(0 , nullptr , (void**)&object->constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));

}

void UpdateObject3d(Object3D* object , ViewProjection viewProjection , XMMATRIX& matProjection) {

	object->worldTransform.UpdateMatWorld();

	//�萔�o�b�t�@�փf�[�^�]��
	object->constMapTransform->mat = object->worldTransform.matWorld;
	object->constMapTransform->mat *= viewProjection.matView;
	object->constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(matProjection);

}

void DrawObject3d(Object3D* object , DX12base dx12base , Model model) {

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , object->constBuffTransform->GetGPUVirtualAddress());

	model.Draw();
}

void MoveObject3d(GameObject3D* object , BYTE key[256]) {
	if (key[DIK_UP] || key[DIK_DOWN] || key[DIK_RIGHT] || key[DIK_LEFT]) {

		if (key[DIK_UP]) {
			object->worldTransform.translation.y += 1.0f;
		}
		if (key[DIK_DOWN]) {
			object->worldTransform.translation.y -= 1.0f;
		}
		if (key[DIK_RIGHT]) {
			object->worldTransform.translation.x += 1.0f;
		}
		if (key[DIK_LEFT]) {
			object->worldTransform.translation.x -= 1.0f;
		}
	}
}

void SetRandomPositionObject3d(Object3D* object) {
	object->worldTransform.translation.x = distPosX(engine);
	object->worldTransform.translation.y = distPosY(engine);
	object->worldTransform.translation.z = distPosZ(engine);
}

void SetRandomRotateObject3d(Object3D* object) {
	object->worldTransform.rotation.x = distRot(engine);
	object->worldTransform.rotation.y = distRot(engine);
	object->worldTransform.rotation.z = distRot(engine);
}

#pragma endregion//�֐��̒�`