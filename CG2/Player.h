#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"
#include <cmath>

class Map;

class Player {
public:
	//�R���X�g���N�^
	Player();
	//�f�X�g���N�^
	~Player();

	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection);

	void Update();

	void Draw();

	void Reset();

	//�A�N�Z�b�T
	void SetMap(Map* map);

	//�����o�֐�
private:
	void Rotate();
	void Move();
	void Colision();

	//�����o�ϐ�
private:
	//�L�[����
	Input& input = Input::GetInstance();

	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject = nullptr;

	Vector3 angle = {};

	Vector3 velocity = {};

	const float accelaration = 0.0025f;

	float moveSpeed = 0;

	const float maxSpeed = 0.5;

	Vector3 target;

	int isDead = false;

	int isHitMap = false;

	Map* map = nullptr;

};
