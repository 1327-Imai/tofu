#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"


class GameScene {

public: // メンバ関数

	//コンストラクタ
	GameScene();

	/// デストラクタ
	~GameScene();

	//初期化
	void Initialize(DX12base* dx12base , Input* input , WinApp* winApp);

	//毎フレーム処理
	void Update();

	//描画処理
	void Draw();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base* dx12base_ = nullptr;
	Input* input_ = nullptr;

	ViewProjection viewProjection_;
	float angle = 0.0f;

	GameObject3D* gameObject = nullptr;
	GameObject3D* gameObject2 = nullptr;

};