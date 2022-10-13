#include "DX12base.h"

void DX12base::Initialize() {
	
	//nullptrチェック
	assert(winApp);

	//デバッグレイヤーの有効化

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	//DirectX初期化処理

	//アダプタの列挙
	//DX6Iファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//パフォーマンスが高いものをから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		 dxgiFactory->EnumAdapterByGpuPreference(i ,
		 DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE ,
		 IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		 i++) {
		//動的配列につかする
		adapters.push_back(tmpAdapter);
	}

	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	//デバイスの生成
	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1 ,
		D3D_FEATURE_LEVEL_12_0 ,
		D3D_FEATURE_LEVEL_11_1 ,
		D3D_FEATURE_LEVEL_11_0 ,
	};

	for (size_t i = 0; i < _countof(levels); i++) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(
			tmpAdapter.Get() ,
			levels[i] ,
			IID_PPV_ARGS(&device)
		);
		if (result == S_OK) {
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	CreateCmdList();
	SetSwapChain();
	DepthTest();
	CreateFence();
}

void DX12base::PreDraw() {
	//バックバッファの番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	//1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();			//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態へ
	commandList->ResourceBarrier(1 , &barrierDesc);

	//2.描画先の変更
	//レンダーターゲットビューのハンドルを取得
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += static_cast<unsigned long long>(bbIndex) * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1 , &rtvHandle , false , &dsvHandle);

	//3.画面クリア
	//レンダーターゲットクリア
	commandList->ClearRenderTargetView(rtvHandle , clearColor , 0 , nullptr);
	
	//深度バッファクリア
	commandList->ClearDepthStencilView(dsvHandle , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);

	//4.描画コマンド
	//ビューポート設定のコマンド
	viewport.Width = winApp->window_width;			//横幅
	viewport.Height = winApp->window_height - 0;	//縦幅

	viewport.TopLeftX = 0;					//左上X
	viewport.TopLeftY = 0;					//左上Y
	viewport.MinDepth = 0.0f;				//最小深度
	viewport.MaxDepth = 1.0f;				//最大深度

	//ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1 , &viewport);

	//シザー矩形
	scissorRect.left = 0;											//切り抜き座標左
	scissorRect.right = scissorRect.left + winApp->window_width;	//切り抜き座標右
	scissorRect.top = 0;											//切り抜き座標上
	scissorRect.bottom = scissorRect.top + winApp->window_height;	//切り抜き座標下

	//シザー矩形コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1 , &scissorRect);

}

void DX12base::PostDraw() {
	//バックバッファの番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	//5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//表示状態へ
	commandList->ResourceBarrier(1 , &barrierDesc);

	//命令のクローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* commandLists[] = {commandList.Get()};
	commandQueue->ExecuteCommandLists(1 , commandLists);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	result = swapChain->Present(1 , 0);
	assert(SUCCEEDED(result));

	//コマンドの実行完了を待つ
	commandQueue->Signal(fence.Get() , ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr , false , false , nullptr);
		fence->SetEventOnCompletion(fenceVal , event);
		if (event != 0) {
			WaitForSingleObject(event , INFINITE);
			CloseHandle(event);
		}
	}

	//キューをクリア
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	//再びコマンドリストをためる準備
	result = commandList->Reset(cmdAllocator.Get() , nullptr);

}

void DX12base::CreateCmdList() {

	//コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		IID_PPV_ARGS(&cmdAllocator)
	);
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device->CreateCommandList(
		0 ,
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		cmdAllocator.Get() , nullptr ,
		IID_PPV_ARGS(&commandList)
	);
	assert(SUCCEEDED(result));

	//コマンドキュー
	//コマンドキューを生成
	result = device->CreateCommandQueue(
		&commandQueueDesc ,
		IID_PPV_ARGS(&commandQueue)
	);
	assert(SUCCEEDED(result));

}

void DX12base::SetSwapChain() {

	swapChainDesc.Width = winApp->window_width;
	swapChainDesc.Height = winApp->window_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//色情報の書式
	swapChainDesc.SampleDesc.Count = 1;								//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				//バックバッファ用
	swapChainDesc.BufferCount = 2;									//バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			//フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get() ,
		winApp->hwnd ,
		&swapChainDesc ,
		nullptr ,
		nullptr ,
		&swapChain1
	);
	assert(SUCCEEDED(result));

	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1.As(&swapChain);

	//デスクリプタヒープ
	//デスクリプタヒープの設定
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc , IID_PPV_ARGS(&rtvHeap));

	//バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	//レンダービューターゲット
	//スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		//スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i , IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//レンダービューターゲットの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//レンダービューターゲットの生成
		device->CreateRenderTargetView(backBuffers[i].Get() , &rtvDesc , rtvHandle);

	}

}

void DX12base::DepthTest() {
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp->window_width;		//レンダーターゲットに合わせる
	depthResourceDesc.Height = winApp->window_height;	//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度値用ヒーププロパティ
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//リソース生成
	result = device->CreateCommittedResource(
		&depthHeapProp ,
		D3D12_HEAP_FLAG_NONE ,
		&depthResourceDesc ,
		D3D12_RESOURCE_STATE_DEPTH_WRITE ,	//深度値書き込みに使用
		&depthClearValue ,
		IID_PPV_ARGS(&depthBuff)
	);

	//深度ビュー用デスクリプタヒープ
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは一つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc , IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get() ,
		&dsvDesc ,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DX12base::CreateFence(){

	//フェンスの生成
	result = device->CreateFence(fenceVal , D3D12_FENCE_FLAG_NONE , IID_PPV_ARGS(&fence));

	assert(SUCCEEDED(result));

}

//アクセッサ
ComPtr<ID3D12GraphicsCommandList> DX12base::GetCmdList() {
	return commandList.Get();
}

ComPtr<ID3D12Device> DX12base::GetDevice() {
	return device.Get();
}

void DX12base::SetWinApp(WinApp* winApp) {
	this->winApp = winApp;
}