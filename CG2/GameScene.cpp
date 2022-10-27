#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete player;
	delete map;
	delete goal;
	delete particle;
	
	delete tutorialFloor;
	delete stageFloor;

	delete title_;
	delete clear_;
	delete gameOver_;
	delete spaceToContinue_;
	delete spaceToReturnTitle_;
	
	delete num0_;
	delete num1_;
	delete num2_;
	delete num3_;
	delete num4_;
	delete num5_;
	delete num6_;
	delete num7_;
	delete num8_;
	delete num9_;
	delete num10_;

	soundManager_.SoundUnload(soundData1);
	soundManager_.SoundUnload(selectSound);
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
	goal->Initialize(&viewProjection_ , &matProjection_);

	player = new Player();
	player->Initialize(&viewProjection_ , &matProjection_);
	player->SetMap(map);
	player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_ , &matProjection_ , player);

	tutorialFloor = new GameObject3D;
	tutorialFloor->PreLoadModel("Resources/stageFloor.obj");
	tutorialFloor->PreLoadTexture(L"Resources/tutorialFloor.png");
	tutorialFloor->SetViewProjection(&viewProjection_);
	tutorialFloor->SetMatProjection(&matProjection_);
	tutorialFloor->Initialize();
	tutorialFloor->worldTransform.translation.x = -1.0;
	tutorialFloor->worldTransform.translation.z = +1.0;
	tutorialFloor->worldTransform.translation.y = -1.0;
	tutorialFloor->worldTransform.rotation.y = MathFunc::Utility::Deg2Rad(90);
	tutorialFloor->Update();

	stageFloor = new GameObject3D;
	//stageFloor->PreLoadModel("Resources/stageFloor.obj");
	stageFloor->PreLoadTexture(L"Resources/stageFloor.png");
	stageFloor->SetViewProjection(&viewProjection_);
	stageFloor->SetMatProjection(&matProjection_);
	stageFloor->Initialize();
	stageFloor->worldTransform.translation.x = -1.0;
	stageFloor->worldTransform.translation.z = +1.0;
	stageFloor->worldTransform.translation.y = -1.0;
	stageFloor->worldTransform.rotation.y = MathFunc::Utility::Deg2Rad(90);
	stageFloor->Update();

	stage = 1;

	Sprite::LoadTexture(1 , L"Resources/title.png");
	Sprite::LoadTexture(2 , L"Resources/GameClear.png");
	Sprite::LoadTexture(3 , L"Resources/GameOver.png");
	Sprite::LoadTexture(4 , L"Resources/Space_to_continue.png");
	Sprite::LoadTexture(5 , L"Resources/Space_to_return_title.png");
	Sprite::LoadTexture(6 , L"Resources/0.png");
	Sprite::LoadTexture(7 , L"Resources/1.png");
	Sprite::LoadTexture(8 , L"Resources/2.png");
	Sprite::LoadTexture(9 , L"Resources/3.png");
	Sprite::LoadTexture(10 , L"Resources/4.png");
	Sprite::LoadTexture(11 , L"Resources/5.png");
	Sprite::LoadTexture(12 , L"Resources/6.png");
	Sprite::LoadTexture(13 , L"Resources/7.png");
	Sprite::LoadTexture(14 , L"Resources/8.png");
	Sprite::LoadTexture(15 , L"Resources/9.png");

	title_ = Sprite::Create(1 , {0 , 0});
	clear_ = Sprite::Create(2 , {0 , 0});
	gameOver_ = Sprite::Create(3 , {0 , 0});
	spaceToContinue_ = Sprite::Create(4 , {0 , 0});
	spaceToReturnTitle_ = Sprite::Create(5 , {0 , 0});
	num0_ = Sprite::Create(6 , {32 , 0});
	num1_ = Sprite::Create(7 , {32 , 0});
	num2_ = Sprite::Create(8 , {32 , 0});
	num3_ = Sprite::Create(9 , {32 , 0});
	num4_ = Sprite::Create(10 , {32 , 0});
	num5_ = Sprite::Create(11 , {32 , 0});
	num6_ = Sprite::Create(12 , {32 , 0});
	num7_ = Sprite::Create(13 , {32 , 0});
	num8_ = Sprite::Create(14 , {32 , 0});
	num9_ = Sprite::Create(15 , {32 , 0});
	num10_ = Sprite::Create(7 , {0 , 0});

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
		soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
		scene_ = GameScene::Scene::Stage;
	}

	break;
	case GameScene::Scene::Stage:
	tutorialFloor->Update();
	stageFloor->Update();

	map->Update();
	player->Update();
	particle->Update();
	goal->Update();


	if (player->GetIsGoal() == true) {
		if (input_.TriggerKey(DIK_SPACE)) {
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
			if (stage < 7) {
				stage++;
				Reset();
			}
			else {
				stage = 1;
				Reset();
				scene_ = Scene::Title;
			}
			player->SetIsGoal(false);
		}
	}

	if (player->GetIsDead() == true && particle->GetIsDead() == true) {
		if (gameoverTimer <= 0) {
			gameoverTimer = 600;
		}
		else {
			gameoverTimer--;
			if (gameoverTimer <= 0) {
				stage = 1;
				Reset();
				scene_ = Scene::Title;
			}
		}

		if (input_.TriggerKey(DIK_SPACE)) {
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), selectSound, false, 0.01f);
			gameoverTimer = 0;
			Reset();
		}
	}
	break;
	}


}

void GameScene::Draw() {
	//3D描画
	switch (scene_) {
	case GameScene::Scene::Title:

	break;
	case GameScene::Scene::Stage:

	if (stage == 1) {
		tutorialFloor->Draw();
	}
	else {
		stageFloor->Draw();
	}
	map->Draw();
	player->Draw();
	particle->Draw();
	goal->Draw();

	break;
	}

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());
	switch (scene_) {
	case GameScene::Scene::Title:

	title_->Draw();

	break;
	case GameScene::Scene::Stage:

	if (player->GetIsGoal() == true) {
		clear_->Draw();
		if (stage < 7) {
			spaceToContinue_->Draw();
		}
		else {
			spaceToReturnTitle_->Draw();
		}
	}

	if (player->GetIsDead() == true && particle->GetIsDead() == true) {
		gameOver_->Draw();
		spaceToContinue_->Draw();

		if (gameoverTimer / 60 == 0) {
			num0_->Draw();
		}
		if (gameoverTimer / 60 == 1) {
			num1_->Draw();
		}
		if (gameoverTimer / 60 == 2) {
			num2_->Draw();
		}
		if (gameoverTimer / 60 == 3) {
			num3_->Draw();
		}
		if (gameoverTimer / 60 == 4) {
			num4_->Draw();
		}
		if (gameoverTimer / 60 == 5) {
			num5_->Draw();
		}
		if (gameoverTimer / 60 == 6) {
			num6_->Draw();
		}
		if (gameoverTimer / 60 == 7) {
			num7_->Draw();
		}
		if (gameoverTimer / 60 == 8) {
			num8_->Draw();
		}
		if (gameoverTimer / 60 == 9) {
			num9_->Draw();
		}
		if (gameoverTimer / 60 == 10) {
			num10_->Draw();
			num0_->Draw();
		}

	}

	break;
	}

	Sprite::PostDraw();

}

void GameScene::Reset() {

	player->Reset();
	particle->Reset();
	map->LoadMap(stage , &viewProjection_ , &matProjection_);

}