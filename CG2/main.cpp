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

#pragma region//�֐��̃v���g�^�C�v�錾
//�E�B���h�E�v���V�[�V��
LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

void MoveObject3d(GameObject3D* object , BYTE key[256]);

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


	GameObject3D gameObject;

	gameObject.SetDX12Base(&dx12base);
	gameObject.PreLoadTexture(L"Resources/texture.jpg");
	gameObject.Initialize();
	gameObject.SetViewProjection(&viewProjection_);
	gameObject.SetMatProjection(&matProjection);

	GameObject3D gameObject2;

	gameObject2.SetDX12Base(&dx12base);
	gameObject2.Initialize();
	gameObject2.SetViewProjection(&viewProjection_);
	gameObject2.SetMatProjection(&matProjection);


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
#pragma endregion//�֐��̒�`