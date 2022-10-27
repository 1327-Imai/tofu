#include "Particle.h"
#include <random>

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> distRot(MathFunc::Utility::Deg2Rad(-60) , MathFunc::Utility::Deg2Rad(60));

Particle::Particle() {

}

Particle::~Particle() {
	for (int i = 0; i < particleValue; i++) {
		delete gameObject[i];
	}
}

void Particle::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection , Player* player) {
	soundManager_.Initialize();
	for (int i = 0; i < particleValue; i++) {

		gameObject[i] = new GameObject3D;
		gameObject[i]->SetViewProjection(viewProjection);
		gameObject[i]->SetMatProjection(matProjection);
		gameObject[i]->Initialize();

		gameObject[i]->Update();

		isDead[i] = true;

		Reset();

	}

	this->player = player;

}

void Particle::Update() {

	if (player->GetIsDead() == true) {
		if (isPlayingBGM == false) {
			//音声再生
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get() , soundData1 , false , 0.01f);
			isPlayingBGM = true;
		}
		for (int i = 0; i < particleValue; i++) {
			if (particleOff[i] == true) {

				angle[i] = player->GetAngle();
				gameObject[i]->worldTransform = player->GetWorldTransform();


				gameObject[i]->worldTransform.scale = {0.5 , 0.5 , 0.5};

				velocity[i] = {
					speed * cosf(angle[i].y) ,
					0 ,
					speed * -sinf(angle[i].y)
				};

				gameObject[i]->worldTransform.rotation = {distRot(engine) , distRot(engine) , distRot(engine)};

				MathFunc::Affine::SetMatRotation(gameObject[i]->worldTransform.matWorld , gameObject[i]->worldTransform.rotation);

				velocity[i] = MathFunc::Utility::MulVector3AndMatrix4(velocity[i] , gameObject[i]->worldTransform.matWorld);

				particleOff[i] = false;
				isDead[i] = false;
				timer[i] = 60;
			}
			else {
				gameObject[i]->worldTransform.translation += velocity[i];
				gameObject[i]->worldTransform.scale = {0.5f * timer[i] / 60 , 0.5f * timer[i] / 60 , 0.5f * timer[i] / 60};

				gameObject[i]->Update();
				timer[i]--;
				if (timer[i] < 0) {
					
					isDead[i] = true;
				}
			}
		}

	}

}

void Particle::Draw() {

	for (int i = 0; i < particleValue; i++) {
		if (isDead[i] == false && particleOff[i] == false) {

			gameObject[i]->Draw();

		}
	}

}

void Particle::Reset() {
	for (int i = 0; i < particleValue; i++) {
	
		particleOff[i] = true;
		isPlayingBGM = false;
	}
}

int Particle::GetIsDead() {
	return isDead[0];
}