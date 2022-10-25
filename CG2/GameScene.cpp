#include "GameScene.h"
#include <cassert>

void MoveObject3d(GameObject3D* object , BYTE key[256]);

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//delete dx12base_;
	delete input_;

	delete gameObject;
	delete gameObject2;

}

void GameScene::Initialize(DX12base* dx12base, Input* input, WinApp* winApp)
{
	// nullptrチェック
	assert(dx12base);
	assert(input);

	dx12base_ = dx12base;
	input_ = input;

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);
	
	viewProjection_.Initialize();

	gameObject = new GameObject3D;
	gameObject->PreLoadTexture(L"Resources/texture.jpg");
	gameObject->Initialize();
	gameObject->SetViewProjection(&viewProjection_);
	gameObject->SetMatProjection(&matProjection_);

	gameObject2 = new GameObject3D;
	gameObject2->Initialize();
	gameObject2->SetViewProjection(&viewProjection_);
	gameObject2->SetMatProjection(&matProjection_);

	//XAudioエンジンのインスタンスを生成
	soundManager_.Initialize();
}

void GameScene::Update()
{
	if (isPlayingBGM == false) {
		//音声再生
		soundManager_.SoundPlayWave(soundManager_.xAudio2.Get() , soundData1 , false , 0.01f);
		isPlayingBGM = true;
	}

	//ビュー変換
	//いずれかのキーを押していたら
	if (input_->PushKey(DIK_D) || input_->PushKey(DIK_A)) {

		//押したキーに応じてangleを増減させる
		if (input_->PushKey(DIK_D)) {
			angle += XMConvertToRadians(1.0f);
		}
		if (input_->PushKey(DIK_A)) {
			angle -= XMConvertToRadians(1.0f);
		}

		//angleラジアンだけY軸周りに回転。半径は - 100
		viewProjection_.eye.x = -100 * sinf(angle);
		viewProjection_.eye.z = -100 * cosf(angle);

		//ビュー変換行列を作り直す
		viewProjection_.UpdateView();

	}

	MoveObject3d(gameObject , input_->key);

	gameObject->Update();
	gameObject2->Update();

	//シーン管理
	switch (scene_)
	{
	case GameScene::Scene::Title:


		break;
	case GameScene::Scene::Stage:


		break;
	case GameScene::Scene::Pose:


		break;
	case GameScene::Scene::Clear:


		break;
	case GameScene::Scene::Over:


		break;

	default:
		break;
	}


}

void GameScene::Draw()
{
	gameObject->Draw();
	gameObject2->Draw();
	switch (scene_)
	{
	case GameScene::Scene::Title:


		break;
	case GameScene::Scene::Stage:


		break;
	case GameScene::Scene::Pose:


		break;
	case GameScene::Scene::Clear:


		break;
	case GameScene::Scene::Over:


		break;

	default:
		break;
	}

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