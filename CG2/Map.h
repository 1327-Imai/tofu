#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include <vector>

class Map {
public:
	//�R���X�g���N�^
	Map();

	//�f�X�g���N�^
	~Map();

	//�����o�֐�
	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection);

	void Update();

	void Draw();

	//�A�N�Z�b�T
	int GetObjectCount();
	WorldTransform GetWorldTransform(int ObjectNum);

private:
	static const int mapWitdh = 50;
	static const int mapHeight = 50;

	int map[mapWitdh][mapHeight] = {0};

	//�Q�[���I�u�W�F�N�g
	std::vector<GameObject3D*> gameObject;

	int objectCount;

};

