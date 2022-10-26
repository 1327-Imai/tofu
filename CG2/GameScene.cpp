#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete player;
	delete map;
	delete goal;
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	viewProjection_.Initialize();
	viewProjection_.eye = {0 , 100 , -100};


	//XAudioエンジンのインスタンスを生成
	soundManager_.Initialize();

	map = new Map();
	map->Initialize(&viewProjection_ , &matProjection_);

	goal = new Goal;
	goal->Initialize(&viewProjection_,&matProjection_);

	player = new Player();
	player->Initialize(&viewProjection_ , &matProjection_);
	player->SetMap(map);
	player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_ , &matProjection_ , player);

	stage = 1;
}

void GameScene::Update() {
	if (isPlayingBGM == false) {
		//音声再生
		//soundManager_.SoundPlayWave(soundManager_.xAudio2.Get() , soundData1 , false , 0.01f);
		isPlayingBGM = true;
	}
	viewProjection_.UpdateView();

	//シーン管理
	switch (scene_) {
	case GameScene::Scene::Title:

	if (input_.PushKey(DIK_SPACE)) {
		scene_ = GameScene::Scene::Stage;
	}

	break;
	case GameScene::Scene::Stage:

	map->Update();
	player->Update();
	particle->Update();
	goal->Update();

	if (player->GetIsGoal() == true) {
		if (stage <= 7) {
			stage++;
			Reset();
		}
		else {
			scene_ = Scene::Clear;
		}
		player->SetIsGoal(false);
	}

	if (input_.TriggerKey(DIK_1)) {
		stage = 1;
		Reset();
	}
	else if (input_.TriggerKey(DIK_2)) {
		stage = 2;
		Reset();
	}
	else if (input_.TriggerKey(DIK_3)) {
		stage = 3;
		Reset();
	}
	else if (input_.TriggerKey(DIK_4)) {
		stage = 4;
		Reset();
	}
	else if (input_.TriggerKey(DIK_5)) {
		stage = 5;
		Reset();
	}
	else if (input_.TriggerKey(DIK_6)) {
		stage = 6;
		Reset();
	}
	else if (input_.TriggerKey(DIK_7)) {
		stage = 7;
		Reset();
	}

	if (input_.TriggerKey(DIK_R)) {
		Reset();
	}

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

void GameScene::Draw() {
	switch (scene_) {
	case GameScene::Scene::Title:


	break;
	case GameScene::Scene::Stage:

	map->Draw();
	player->Draw();
	particle->Draw();
	goal->Draw();

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

void GameScene::Reset() {

	player->Reset();
	particle->Reset();
	map->LoadMap(stage,&viewProjection_,&matProjection_);

}