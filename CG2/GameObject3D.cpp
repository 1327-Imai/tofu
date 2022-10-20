#include "GameObject3D.h"

//�O���錾�̒�`
class DX12base {
public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	ComPtr<ID3D12Device> GetDevice();
};
class ViewProjection {
public:
	Matrix4 matView;
};

//�����o�֐�
void GameObject3D::Initialize() {

	HRESULT result;

	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));

	//���[���h�ϊ��̏�����
	worldTransform.initialize();

	//���f���̏�����
	model.LoadModel();
	model.Initialize();

}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->mat = worldTransform.matWorld;
	constMapTransform->mat *= viewProjection->matView;
	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);

}

void GameObject3D::Draw() {

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dx12base->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	model.Draw();

}

//�A�N�Z�b�T
void GameObject3D::SetDX12Base(DX12base* dx12base) {
	this->dx12base = dx12base;
	model.SetDx12Base(dx12base);
}

void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
	this->viewProjection = viewProjection;
}

void GameObject3D::SetMatProjection(XMMATRIX* matProjection) {
	this->matProjection = matProjection;
}