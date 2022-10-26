#include "Map.h"
Map::Map() {

}

Map::~Map() {
	for (int i = 0; i < objectCount; i++) {
		delete gameObject[i];
	}
}

void Map::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection) {

	LoadMap(1 , viewProjection , matProjection);

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

void Map::LoadMap(int stageNum , ViewProjection* viewProjection , XMMATRIX* matProjection) {

	for (int i = 0; i < objectCount; i++) {
		delete gameObject[i];
	}

	switch (stageNum) {
	case 1:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage1[i][j];
		}
	}
	break;

	case 2:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage2[i][j];
		}
	}
	break;

	case 3:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage3[i][j];
		}
	}
	break;

	case 4:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage4[i][j];
		}
	}
	break;

	case 5:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage5[i][j];
		}
	}
	break;

	case 6:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage6[i][j];
		}
	}
	break;

	case 7:
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = stage7[i][j];
		}
	}
	break;

	}

	objectCount = 0;

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
		
			objectID[i][j] = -1;
		
		}
	}

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (map[i][j] == 1) {
				objectCount++;
			}
		}
	}

	gameObject.resize((size_t)objectCount + 1);

	int num = 0;
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {

			if (map[i][j] == 1) {
				gameObject[num] = new GameObject3D;
				gameObject[num]->SetViewProjection(viewProjection);
				gameObject[num]->SetMatProjection(matProjection);
				gameObject[num]->Initialize();

				gameObject[num]->worldTransform.translation.x = (j - mapWidth / 2) * 2;
				gameObject[num]->worldTransform.translation.z = (-i + mapHeight / 2) * 2;

				objectID[i][j] = num;

				num++;
			}

		}
	}

}

//アクセッサ
int Map::GetObjectCount() {
	return objectCount;
}

int Map::GetObjectID(int x , int y) {
	return objectID[y][x];
}

WorldTransform Map::GetWorldTransform(int ObjectNum) {
	return gameObject[ObjectNum]->worldTransform;
}