#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"


class GameScene {

public: // �����o�֐�

	//�R���X�g���N�^
	GameScene();

	/// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(DX12base* dx12base , Input* input , WinApp* winApp);

	//���t���[������
	void Update();

	//�`�揈��
	void Draw();

private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DX12base* dx12base_ = nullptr;
	Input* input_ = nullptr;

	ViewProjection viewProjection_;
	float angle = 0.0f;

	GameObject3D* gameObject = nullptr;
	GameObject3D* gameObject2 = nullptr;

};