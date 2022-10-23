#include "GameObject3D.h"

class ViewProjection {
public:
	Matrix4 matView;
};

//�����o�֐�
void GameObject3D::PreLoadModel(const char* modelFileName) {
	this->modelFileName = modelFileName;
}

//�����o�֐�
void GameObject3D::PreLoadTexture(const wchar_t* textureFileName) {
	this->textureFileName = textureFileName;
}

void GameObject3D::Initialize() {

	InitializeConstMapTransform();
	InitializeConstMapMaterial();

	//���[���h�ϊ��̏�����
	worldTransform.initialize();

	//���f���̏�����
	model.LoadModel(modelFileName);
	model.Initialize();

	textrue.LoadTexture(textureFileName);
	textrue.SetModel(&model);
	textrue.CreateSRV();
}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->mat = worldTransform.matWorld;
	constMapTransform->mat *= viewProjection->matView;
	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);

}

void GameObject3D::Draw() {

	//���_�o�b�t�@�\�r���[���Z�b�g����R�}���h
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	textrue.Draw();

	model.Draw();

}

void GameObject3D::InitializeConstMapTransform() {
	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	cbTransformHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	cbTransformResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbTransformResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbTransformResourceDesc.Height = 1;
	cbTransformResourceDesc.DepthOrArraySize = 1;
	cbTransformResourceDesc.MipLevels = 1;
	cbTransformResourceDesc.SampleDesc.Count = 1;
	cbTransformResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbTransformHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbTransformResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));

}

void GameObject3D::InitializeConstMapMaterial() {

	HRESULT result;

	//�萔�o�b�t�@�̐���
//�q�[�v�ݒ�
	cbMaterialHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};
	cbMaterialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMaterialResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbMaterialResourceDesc.Height = 1;
	cbMaterialResourceDesc.DepthOrArraySize = 1;
	cbMaterialResourceDesc.MipLevels = 1;
	cbMaterialResourceDesc.SampleDesc.Count = 1;
	cbMaterialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbMaterialHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbMaterialResourceDesc ,	//���\�[�X�ݒ�
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

}


//�A�N�Z�b�T
void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
	this->viewProjection = viewProjection;
}

void GameObject3D::SetMatProjection(XMMATRIX* matProjection) {
	this->matProjection = matProjection;
}