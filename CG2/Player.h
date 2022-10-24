#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"

class Map;

class Player {
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection);

	void Update();

	void Draw();

	//アクセッサ
	void SetMap(Map* map);

	//メンバ関数
private:
	void Rotate();
	void Move();
	void Colision();

	//メンバ変数
private:
	//キー入力
	Input& input = Input::GetInstance();

	//ゲームオブジェクト
	GameObject3D* gameObject = nullptr;

	Vector3 angle = {};

	Vector3 velocity = {};

	const float accelaration = 0.005f;

	float moveSpeed = 0;

	const float maxSpeed = 1.0;

	Vector3 target;

	int isDead = false;

	int isHitMap = false;

	Map* map = nullptr;

};

