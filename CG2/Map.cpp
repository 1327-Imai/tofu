#include "Map.h"
Map::Map() {

}

Map::~Map() {
	for (int i = 0; i < objectCount; i++) {
		delete gameObject[i];
	}
}

void Map::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection) {

	objectCount = 0;

	for (int i = 0; i < mapWitdh; i++) {
		for (int j = 0; j < mapHeight; j++) {

			if (i == 0 || i == mapWitdh - 1) {
				map[i][j] = 1;
				objectCount++;
			}
			else if (j == 0 || j == mapHeight - 1) {
				map[i][j] = 1;
				objectCount++;
			}
			else {
				map[i][j] = 0;
			}
		}
	}

	gameObject.resize((size_t)objectCount + 1);

	int num = 0;
	for (int i = 0; i < mapWitdh; i++) {
		for (int j = 0; j < mapHeight; j++) {

			if (map[i][j] == 1) {
				gameObject[num] = new GameObject3D;
				gameObject[num]->SetViewProjection(viewProjection);
				gameObject[num]->SetMatProjection(matProjection);
				gameObject[num]->Initialize();

				gameObject[num]->worldTransform.translation.x = (i - mapWitdh / 2) * 2;
				gameObject[num]->worldTransform.translation.z = (j - mapHeight / 2) * 2;

				num++;
			}

		}
	}

}

void Map::Update() {

	for (int i = 0; i < objectCount; i++) {
		gameObject[i]->Update();
	}

}

void Map::Draw() {

	for (int i = 0; i < objectCount; i++) {
		gameObject[i]->Draw();
	}

}

//アクセッサ
int Map::GetObjectCount() {
	return objectCount;
}

WorldTransform Map::GetWorldTransform(int ObjectNum) {
	return gameObject[ObjectNum]->worldTransform;
}