#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Texture.h"

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
	void PreLoadModel(const char* modelFileName);
	void PreLoadTexture(const wchar_t* textureFileName);

	void Initialize();

	void Update();

	void Draw();

	//�A�N�Z�b�T
	void SetDX12Base(DX12base* dx12base);
	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);

private:
	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	//�\����
private:
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


private:
	//���f��
	Model model;
	//���f���̃t�@�C����
	const char* modelFileName = nullptr;

	//�e�N�X�`��
	Texture textrue;
	//���f���̃t�@�C����
	const wchar_t* textureFileName = nullptr;

	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�s���O(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbTransformHeapProp;
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbTransformResourceDesc;
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	XMMATRIX* matProjection;

	//DirectX��b����
	DX12base* dx12base;
	//�r���[�v���W�F�N�V����
	ViewProjection* viewProjection;

};

