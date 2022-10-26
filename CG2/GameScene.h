#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"

#include "Audio.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include "Sprite.h"

#include "Player.h"
#include "Map.h"
#include "Goal.h"
#include "Particle.h"

class GameScene {

public: // メンバ関数

	//コンストラクタ
	GameScene();

	/// デストラクタ
	~GameScene();

	//初期化
	void Initialize(WinApp* winApp);

	//毎フレーム処理
	void Update();

	//描画処理
	void Draw();

	void Reset();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	SoundManager soundManager_;

	ViewProjection viewProjection_;

	//音声読み込み
	SoundData soundData1 = soundManager_.SoundLoadWave("Resources/Alarm01.wav");

	bool isPlayingBGM = false;

	Player* player = nullptr;

	Particle* particle = nullptr;

	Map* map = nullptr;

	Goal* goal = nullptr;

	GameObject3D* tutorialFloor = nullptr;
	GameObject3D* stageFloor = nullptr;

	//シーン管理
	enum class Scene
	{
		Title,//タイトル
		Stage,//ステージ
	};

	Scene scene_ = Scene::Title;

	int stage = 0;

	int gameoverTimer = 0;

	Sprite* title_ = nullptr;
	Sprite* clear_ = nullptr;
	Sprite* gameOver_ = nullptr;
	Sprite* spaceToContinue_ = nullptr;
	Sprite* spaceToReturnTitle_ = nullptr;
	Sprite* num0_ = nullptr;
	Sprite* num1_ = nullptr;
	Sprite* num2_ = nullptr;
	Sprite* num3_ = nullptr;
	Sprite* num4_ = nullptr;
	Sprite* num5_ = nullptr;
	Sprite* num6_ = nullptr;
	Sprite* num7_ = nullptr;
	Sprite* num8_ = nullptr;
	Sprite* num9_ = nullptr;
	Sprite* num10_ = nullptr;

};