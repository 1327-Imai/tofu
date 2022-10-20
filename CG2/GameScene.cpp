#include "GameScene.h"
#include <cassert>

void MoveObject3d(GameObject3D* object , BYTE key[256]);

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete dx12base_;
	delete input_;

	delete gameObject;
	delete gameObject2;

}

void GameScene::Initialize(DX12base* dx12base, Input* input, WinApp* winApp)
{
	// nullptr�`�F�b�N
	assert(dx12base);
	assert(input);

	dx12base_ = dx12base;
	input_ = input;

	//�������e�ϊ��s��̌v�Z
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);
	
	viewProjection_.Initialize();


	gameObject = new GameObject3D;
	gameObject->SetDX12Base(dx12base);
	gameObject->PreLoadTexture(L"Resources/texture.jpg");
	gameObject->Initialize();
	gameObject->SetViewProjection(&viewProjection_);
	gameObject->SetMatProjection(&matProjection);

	gameObject2 = new GameObject3D;
	gameObject2->SetDX12Base(dx12base);
	gameObject2->Initialize();
	gameObject2->SetViewProjection(&viewProjection_);
	gameObject2->SetMatProjection(&matProjection);
}

void GameScene::Update()
{
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

		//angle���W�A������Y������ɉ�]�B���a�� - 100
		viewProjection_.eye.x = -100 * sinf(angle);
		viewProjection_.eye.z = -100 * cosf(angle);

		//�r���[�ϊ��s�����蒼��
		viewProjection_.UpdateView();

	}

	MoveObject3d(gameObject , input_->key);

	gameObject->Update();
	gameObject2->Update();

}

void GameScene::Draw()
{
	gameObject->Draw();
	gameObject2->Draw();

}


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