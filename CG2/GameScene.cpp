#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete player;
	delete map;
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

	player = new Player();
	player->Initialize(&viewProjection_ , &matProjection_);
	player->SetMap(map);
}

void GameScene::Update() {
	if (isPlayingBGM == false) {
		//音声再生
		//soundManager_.SoundPlayWave(soundManager_.xAudio2.Get() , soundData1 , false , 0.01f);
		isPlayingBGM = true;
	}
	viewProjection_.UpdateView();


	map->Update();
	player->Update();
}

void GameScene::Draw() {

	map->Draw();
	player->Draw();
}