#pragma once
#include "GameObject3D.h"
#include "Player.h"
#include "Audio.h"

class Particle {
public:

	Particle();

	~Particle();

	void Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection,Player* player);

	void Update();

	void Draw();

	void Reset();

	int GetIsDead();

private:

	static const int particleValue = 20;

	GameObject3D* gameObject[particleValue];

	Vector3 velocity[particleValue] = {};
	Vector3 angle[particleValue] = {};

	const float speed = 1;

	int isDead[particleValue] = {0};

	int timer[particleValue] = {0};

	int particleOff[particleValue] = {0};

	Player* player = nullptr;
	SoundManager soundManager_;
	//âπê∫ì«Ç›çûÇ›
	SoundData soundData1 = soundManager_.SoundLoadWave("Resources/death.wav");
	bool isPlayingBGM = false;
};

