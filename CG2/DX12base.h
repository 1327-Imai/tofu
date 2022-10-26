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
	//メンバ関数
	//初期化処理
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
	//アクセッサ
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();

	ComPtr<ID3D12Device> GetDevice();

	void SetWinApp(WinApp* winApp);

	//メンバ変数
private:
	//DirectX初期化処理
	ComPtr<ID3D12Debug> debugController;
	HRESULT result;
	ComPtr<ID3D12Device> device = nullptr;
	ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;

	//アダプターの列挙例
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	D3D_FEATURE_LEVEL featureLevel;

	//コマンドキュー
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

#pragma region//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	
	//IDXGISwapChain1のComPtrを用意
	ComPtr<IDXGISwapChain1> swapChain1;

	//デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

#pragma endregion

#pragma region//深度テスト
	//デスクリプタ
	D3D12_RESOURCE_DESC depthResourceDesc{};

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};

	//リソース
	ComPtr<ID3D12Resource> depthBuff = nullptr;

	//深度ビュー用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

#pragma endregion

#pragma region//フェンス

	//フェンスの生成
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

#pragma endregion

#pragma region//描画処理

	//バックバッファ
	UINT bbIndex;

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};

	//レンダーターゲットビューのハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//クリアカラー
	FLOAT clearColor[4] = {0.25f , 0.25f , 0.25f , 0.0f};

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};

#pragma endregion

	//前方宣言
	WinApp* winApp = nullptr;
};

