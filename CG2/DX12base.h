#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXmath.h>
#include <wrl.h>

#include <cassert>
#include <vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "WinApp.h"

#include "Singleton.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class DX12base :public Singleton<DX12base>{

	friend Singleton<DX12base>;

public:
	//�����o�֐�
	//����������
	void Initialize();

	void PreDraw();

	void PostDraw();

private:
	DX12base() {};
	~DX12base() {};

	void CreateCmdList();

	void SetSwapChain();

	void DepthTest();

	void CreateFence();

public:
	//�A�N�Z�b�T
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();

	ComPtr<ID3D12Device> GetDevice();

	void SetWinApp(WinApp* winApp);

	//�����o�ϐ�
private:
	//DirectX����������
	ComPtr<ID3D12Debug> debugController;
	HRESULT result;
	ComPtr<ID3D12Device> device = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;

	//�A�_�v�^�[�̗񋓗�
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	D3D_FEATURE_LEVEL featureLevel;

	//�R�}���h�L���[
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

#pragma region//�X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	
	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapChain1;

	//�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

#pragma endregion

#pragma region//�[�x�e�X�g
	//�f�X�N���v�^
	D3D12_RESOURCE_DESC depthResourceDesc{};

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};

	//���\�[�X
	ComPtr<ID3D12Resource> depthBuff = nullptr;

	//�[�x�r���[�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

#pragma endregion

#pragma region//�t�F���X

	//�t�F���X�̐���
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

#pragma endregion

#pragma region//�`�揈��

	//�o�b�N�o�b�t�@
	UINT bbIndex;

	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc{};

	//�����_�[�^�[�Q�b�g�r���[�̃n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//�N���A�J���[
	FLOAT clearColor[4] = {0.1f , 0.25f , 0.5f , 0.0f};

	//�r���[�|�[�g
	D3D12_VIEWPORT viewport{};

	//�V�U�[��`
	D3D12_RECT scissorRect{};

#pragma endregion

	//�O���錾
	WinApp* winApp = nullptr;
};

