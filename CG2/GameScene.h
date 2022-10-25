#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"

#include "Audio.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "Player.h"
#include "Map.h"

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

	Map* map = nullptr;
  
	//シーン管理
	enum class Scene
	{
		Title,//タイトル
		Stage,//ステージ
		Pose,//ポーズ
		Clear,//クリア
		Over,//ゲームオーバー
	};

	Scene scene_ = Scene::Title;

};