#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include 

class GameScene {

public: // �����o�֐�

	//�R���X�g���N�^
	GameScene();

	/// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(DX12base* DX12base, Input* input,WinApp* winApp);

	//���t���[������
	void Update();

	//�`�揈��
	void Draw();

private: // �����o�ϐ�
	DX12base* dx12base = nullptr;
	Input* input = nullptr;
	GameObject3D* gameObject = nullptr;
	GameObject3D* gameObject2 = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	WinApp* winApp_ = nullptr;

};


