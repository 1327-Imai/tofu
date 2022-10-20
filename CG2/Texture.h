#pragma once
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <cassert>
#include <vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "WinApp.h"

#include <DirectXTex.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Model;
class DX12base;

class Texture {

	//�����o�֐�
public:
	void LoadTexture(const wchar_t* fileName);

	void CreateSRV();

	void Draw();

	//�A�N�Z�b�T
	void SetDx12Base(DX12base* dx12base);

	void SetModel(Model* model);

	//�����o�ϐ�
private:
	//�摜�C���[�W�f�[�^�̍쐬
	TexMetadata metadata;
	ScratchImage scratchImg;

	//�~�b�v�}�b�v
	ScratchImage mipChain;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc;

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//DirectX��b����
	DX12base* dx12base;

	//���f��
	Model* model;

};