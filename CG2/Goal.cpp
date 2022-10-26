#include "Goal.h"

Goal::Goal() {

}

Goal::~Goal() {
	delete gameObject;
}

void Goal::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/black1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = {40 , 60 , 40};
	gameObject->worldTransform.scale = {2 , 2 , 2};

}

void Goal::Update() {

	if (ct <= 0) {
		moveSpeed = g * (timer / 60);

		gameObject->worldTransform.translation.y -= moveSpeed;

		timer++;
		if (gameObject->worldTransform.translation.y < 0) {
			timer = 0;
			gameObject->worldTransform.translation.y = 60;
			ct = 60;
		}
	}
	else {
		ct--;
	}

	gameObject->Update();

}

void Goal::Draw() {
	gameObject->Draw();
}

WorldTransform Goal::GetWorldTransform() {
	return gameObject->worldTransform;
}