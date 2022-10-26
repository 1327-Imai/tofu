#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"

class Map;
class Goal;

class Player {
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection);

	void Update();

	void Draw();

	void Reset();

	//アクセッサ
	void SetMap(Map* map);
	void SetGoal(Goal* goal);

	int GetIsGoal();
	void SetIsGoal(int flag);

	int GetIsDead();

	Vector3 GetAngle();
	
	WorldTransform GetWorldTransform();

	//メンバ関数
private:
	void Rotate();
	void Move();
	void Collision();

	//メンバ変数
private:
	//キー入力
	Input& input = Input::GetInstance();

	//ゲームオブジェクト
	GameObject3D* gameObject = nullptr;

	Vector3 angle = {};

	Vector3 velocity = {};

	const float accelaration = 0.0025f;

	float moveSpeed = 0;

	const float maxSpeed = 0.5;

	Vector3 target;

	int isDead = false;

	int isHitMap = false;

	int isGoal = false;

	Map* map = nullptr;
	Goal* goal = nullptr;
};

