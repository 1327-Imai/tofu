#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include <vector>

class Map {
public:
	//コンストラクタ
	Map();

	//デストラクタ
	~Map();

	//メンバ関数
	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection);

	void Update();

	void Draw();

	//アクセッサ
	int GetObjectCount();
	WorldTransform GetWorldTransform(int ObjectNum);

private:
	static const int mapWitdh = 50;
	static const int mapHeight = 50;

	int map[mapWitdh][mapHeight] = {0};

	//ゲームオブジェクト
	std::vector<GameObject3D*> gameObject;

	int objectCount;

};

