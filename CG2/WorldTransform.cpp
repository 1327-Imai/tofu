#include "WorldTransform.h"

void WorldTransform::initialize() {

	scale = {1 , 1 , 1};
	rotation = {0 , 0 , 0};
	translation = {0 , 0 , 0};
	matWorld.SetIdentityMatrix();

	UpdateMatWorld();

}

//ワールド変換行列の更新
void WorldTransform::UpdateMatWorld() {

	//ワールド変換行列を用意
	Matrix4 affineMat;
	affineMat.SetIdentityMatrix();

	//ワールド変換行列にスケーリング,回転,平行移動の行列を合成
	MathFunc::Affine::SetMatScale(affineMat , scale);
	MathFunc::Affine::SetMatRotation(affineMat , rotation);
	MathFunc::Affine::SetMatTranslation(affineMat , translation);

	//ワールド行列に単位行列を代入
	matWorld.SetIdentityMatrix();

	//行列の計算
	matWorld *= affineMat;

	//もし親がある場合
	if (parent) {
		//親のワールド行列との計算を行う
		matWorld *= parent->matWorld;
	}

}