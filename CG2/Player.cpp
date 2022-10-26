#include "Player.h"

class Map {
public:
	int GetObjectCount();
	int GetObjectID(int x , int y);
	WorldTransform GetWorldTransform(int ObjectNum);
};

class Goal {
public:
	WorldTransform GetWorldTransform();
};

Player::Player() {

}

Player::~Player() {
	delete gameObject;
}

void Player::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Reset();

}

void Player::Update() {

	Rotate();
	Move();
	Collision();

	if (isDead == false) {
		gameObject->Update();
	}
}

void Player::Draw() {
	if (isDead == false) {
		gameObject->Draw();
	}
}

void Player::Reset() {
	gameObject->worldTransform.translation = {-40 , 0 , -40};
	moveSpeed = 0;
	isDead = false;
}

void Player::Rotate() {

	const float rotationSpeed = MathFunc::Utility::Deg2Rad(6.0f);

	Vector3 rotation = {0 , 0 , 0};

	rotation.y = rotationSpeed;

	gameObject->worldTransform.rotation += rotation;

}

void Player::Move() {

	if (input.PushKey(DIK_SPACE)) {
		isHitMap = false;

		angle = gameObject->worldTransform.rotation;

		if (moveSpeed < maxSpeed) {
			moveSpeed += accelaration;

			if (maxSpeed <= moveSpeed) {
				moveSpeed = maxSpeed;
			}
		}
	}
	else {
		if (0 < moveSpeed) {
			moveSpeed -= accelaration;

			if (moveSpeed <= 0) {
				moveSpeed = 0;
			}
		}

	}
	if (isHitMap == false) {
		velocity = {
			moveSpeed * cosf(angle.y) ,
			0 ,
			moveSpeed * -sinf(angle.y)
		};
	}
	else {
		velocity = {
			moveSpeed * -cosf(-angle.y) ,
			0 ,
			moveSpeed * -sinf(-angle.y)
		};
	}

	gameObject->worldTransform.translation += velocity;

}

void Player::Collision() {
	//マップチップ
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {

			int objectId = 0;

			objectId = map->GetObjectID(
				j + (int)(gameObject->worldTransform.translation.x / 2 + 25) ,
				i + (int)(50 - (gameObject->worldTransform.translation.z / 2 + 25))
			);

			if (0 <= objectId) {

				if (map->GetWorldTransform(objectId).translation.x - gameObject->worldTransform.translation.x < 2 &&
					-2 < map->GetWorldTransform(objectId).translation.x - gameObject->worldTransform.translation.x) {
					if (map->GetWorldTransform(objectId).translation.z - gameObject->worldTransform.translation.z < 2 &&
						-2 < map->GetWorldTransform(objectId).translation.z - gameObject->worldTransform.translation.z) {

						if (0.4 < moveSpeed) {
							isDead = true;
						}
						else {
							if (isHitMap == false) {
								isHitMap = true;
							}
							else {
								isHitMap = false;
							}
							gameObject->worldTransform.translation -= velocity;
						}
					}
				}
			}
		}
	}

	//ゴール
	if (goal->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x < 3 &&
		-3 < goal->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x) {
		if (goal->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y < 3 &&
			-3 < goal->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y) {
			if (goal->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z < 3 &&
				-3 < goal->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z) {
			
				isGoal = true;
			
			}
		}
	}
}

//アクセッサ
void Player::SetMap(Map* map) {
	this->map = map;
}

void Player::SetGoal(Goal* goal) {
	this->goal = goal;
}

int Player::GetIsGoal() {
	return isGoal;
}

void Player::SetIsGoal(int flag) {
	isGoal = flag;
}

int Player::GetIsDead() {
	return isDead;
}

Vector3 Player::GetAngle() {
	return angle;
}

WorldTransform Player::GetWorldTransform() {
	return gameObject->worldTransform;
}