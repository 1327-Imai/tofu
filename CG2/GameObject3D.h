#pragma once
#include "WorldTransform.h"
#include "Model.h"

#include "MathFunc.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


//�O���錾
class DX12base;
class ViewProjection;

class GameObject3D {

public:
	//�����o�ϐ�
	void Initialize();

	void Update();

	void Draw();

	//�A�N�Z�b�T
	void SetDX12Base(DX12base* dx12base);
	//void LoadModel(Model* model);
	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);

private:
	//�\����
	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		Vector4 color; //�F(RGBA)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform {
		Matrix4 mat; //3D�ϊ��s��
	};

	//�����o�ϐ�
public:
	//���[���h�ϊ�
	WorldTransform worldTransform;

	//���f���̃t�@�C����
	char* modelFileName = nullptr;

private:
	//���f��
	Model model;

	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�s���O(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	XMMATRIX* matProjection;

	//DirectX��b����
	DX12base* dx12base;
	//�r���[�v���W�F�N�V����
	ViewProjection* viewProjection;

};

