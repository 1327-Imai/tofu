#include "DX12base.h"

void DX12base::Initialize() {
	
	//nullptr�`�F�b�N
	assert(winApp);

	//�f�o�b�O���C���[�̗L����

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif

	//DirectX����������

	//�A�_�v�^�̗�
	//DX6I�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//�p�t�H�[�}���X���������̂����珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		 dxgiFactory->EnumAdapterByGpuPreference(i ,
		 DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE ,
		 IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		 i++) {
		//���I�z��ɂ�����
		adapters.push_back(tmpAdapter);
	}

	//�Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//�A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

	//�f�o�C�X�̐���
	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1 ,
		D3D_FEATURE_LEVEL_12_0 ,
		D3D_FEATURE_LEVEL_11_1 ,
		D3D_FEATURE_LEVEL_11_0 ,
	};

	for (size_t i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(
			tmpAdapter.Get() ,
			levels[i] ,
			IID_PPV_ARGS(&device)
		);
		if (result == S_OK) {
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
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
	//�o�b�N�o�b�t�@�̔ԍ����擾
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();			//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		//�\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ�
	commandList->ResourceBarrier(1 , &barrierDesc);

	//2.�`���̕ύX
	//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += static_cast<unsigned long long>(bbIndex) * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1 , &rtvHandle , false , &dsvHandle);

	//3.��ʃN���A
	//�����_�[�^�[�Q�b�g�N���A
	commandList->ClearRenderTargetView(rtvHandle , clearColor , 0 , nullptr);
	
	//�[�x�o�b�t�@�N���A
	commandList->ClearDepthStencilView(dsvHandle , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);

	//4.�`��R�}���h
	//�r���[�|�[�g�ݒ�̃R�}���h
	viewport.Width = winApp->window_width;			//����
	viewport.Height = winApp->window_height - 0;	//�c��

	viewport.TopLeftX = 0;					//����X
	viewport.TopLeftY = 0;					//����Y
	viewport.MinDepth = 0.0f;				//�ŏ��[�x
	viewport.MaxDepth = 1.0f;				//�ő�[�x

	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetViewports(1 , &viewport);

	//�V�U�[��`
	scissorRect.left = 0;											//�؂蔲�����W��
	scissorRect.right = scissorRect.left + winApp->window_width;	//�؂蔲�����W�E
	scissorRect.top = 0;											//�؂蔲�����W��
	scissorRect.bottom = scissorRect.top + winApp->window_height;	//�؂蔲�����W��

	//�V�U�[��`�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetScissorRects(1 , &scissorRect);

}

void DX12base::PostDraw() {
	//�o�b�N�o�b�t�@�̔ԍ����擾
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	//5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			//�\����Ԃ�
	commandList->ResourceBarrier(1 , &barrierDesc);

	//���߂̃N���[�Y
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = {commandList.Get()};
	commandQueue->ExecuteCommandLists(1 , commandLists);

	//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = swapChain->Present(1 , 0);
	assert(SUCCEEDED(result));

	//�R�}���h�̎��s������҂�
	commandQueue->Signal(fence.Get() , ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr , false , false , nullptr);
		fence->SetEventOnCompletion(fenceVal , event);
		if (event != 0) {
			WaitForSingleObject(event , INFINITE);
			CloseHandle(event);
		}
	}

	//�L���[���N���A
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	//�ĂуR�}���h���X�g�����߂鏀��
	result = commandList->Reset(cmdAllocator.Get() , nullptr);

}

void DX12base::CreateCmdList() {

	//�R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		IID_PPV_ARGS(&cmdAllocator)
	);
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�𐶐�
	result = device->CreateCommandList(
		0 ,
		D3D12_COMMAND_LIST_TYPE_DIRECT ,
		cmdAllocator.Get() , nullptr ,
		IID_PPV_ARGS(&commandList)
	);
	assert(SUCCEEDED(result));

	//�R�}���h�L���[
	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(
		&commandQueueDesc ,
		IID_PPV_ARGS(&commandQueue)
	);
	assert(SUCCEEDED(result));

}

void DX12base::SetSwapChain() {

	swapChainDesc.Width = winApp->window_width;
	swapChainDesc.Height = winApp->window_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				//�F���̏���
	swapChainDesc.SampleDesc.Count = 1;								//�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;									//�o�b�t�@����2�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			//�t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get() ,
		winApp->hwnd ,
		&swapChainDesc ,
		nullptr ,
		nullptr ,
		&swapChain1
	);
	assert(SUCCEEDED(result));

	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapChain1.As(&swapChain);

	//�f�X�N���v�^�q�[�v
	//�f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//�f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc , IID_PPV_ARGS(&rtvHeap));

	//�o�b�N�o�b�t�@
	backBuffers.resize(swapChainDesc.BufferCount);

	//�����_�[�r���[�^�[�Q�b�g
	//�X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++) {
		//�X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i , IID_PPV_ARGS(&backBuffers[i]));
		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//�����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//�����_�[�r���[�^�[�Q�b�g�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//�����_�[�r���[�^�[�Q�b�g�̐���
		device->CreateRenderTargetView(backBuffers[i].Get() , &rtvDesc , rtvHandle);

	}

}

void DX12base::DepthTest() {
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winApp->window_width;		//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = winApp->window_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	result = device->CreateCommittedResource(
		&depthHeapProp ,
		D3D12_HEAP_FLAG_NONE ,
		&depthResourceDesc ,
		D3D12_RESOURCE_STATE_DEPTH_WRITE ,	//�[�x�l�������݂Ɏg�p
		&depthClearValue ,
		IID_PPV_ARGS(&depthBuff)
	);

	//�[�x�r���[�p�f�X�N���v�^�q�[�v
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[�͈��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc , IID_PPV_ARGS(&dsvHeap));

	//�[�x�r���[�쐬
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get() ,
		&dsvDesc ,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DX12base::CreateFence(){

	//�t�F���X�̐���
	result = device->CreateFence(fenceVal , D3D12_FENCE_FLAG_NONE , IID_PPV_ARGS(&fence));

	assert(SUCCEEDED(result));

}

//�A�N�Z�b�T
ComPtr<ID3D12GraphicsCommandList> DX12base::GetCmdList() {
	return commandList.Get();
}

ComPtr<ID3D12Device> DX12base::GetDevice() {
	return device.Get();
}

void DX12base::SetWinApp(WinApp* winApp) {
	this->winApp = winApp;
}