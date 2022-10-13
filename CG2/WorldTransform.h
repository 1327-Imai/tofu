#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "MathFunc.h"

class WorldTransform {
public:
	//メンバ関数
	void initialize();

	//ワールド変換行列の更新
	void UpdateMatWorld();

	//メンバ変数
	//スケール
	Vector3 scale;
	//XYZ軸回転
	Vector3 rotation;
	//ローカル座標
	Vector3 translation;
	//ワールド変換行列
	Matrix4 matWorld;
	//親オブジェクトのワールド変換のポインタ
	WorldTransform* parent = nullptr;

};

