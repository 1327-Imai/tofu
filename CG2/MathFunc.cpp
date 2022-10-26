#include "MathFunc.h"

//スケーリング行列を設定する関数
void MathFunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matScale;

}

//回転行列を設定する関数
void MathFunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	//回転行列を宣言
	//Z軸回転
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , -(float)sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//平行移動行列の設定をする関数
void MathFunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//ワールド変換行列を生成する関数
//スケーリング行列を生成する関数
Matrix4 MathFunc::Affine::CreateMatScale(Vector3 scale) {

	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	return matScale;

}

//回転行列を生成する関数
Matrix4 MathFunc::Affine::CreateMatRotation(Vector3 rotation) {

	//回転行列を宣言
	Matrix4 matRot;
	matRot.SetIdentityMatrix();

	//Z軸回転
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , (float)-sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}


//平行移動行列の生成をする関数
Matrix4 MathFunc::Affine::CreateMatTranslation(Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	return matTranslation;
}

#pragma endregion

//XMMATRIXをMatrix4に変換する関数
Matrix4 MathFunc::Utility::ConvertXMMATRIXtoMatrix4(XMMATRIX xmMatrix) {
	Matrix4 matrix4;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			matrix4.m[i][j] = xmMatrix.r[i].m128_f32[j];

		}
	}

	return matrix4;
}

//ビュー行列を生成する関数
Matrix4 MathFunc::Utility::CreatMatView(Vector3 eye , Vector3 target , Vector3 up){

	Matrix4 matView;

	XMFLOAT3 xmEye;
	xmEye.x = eye.x;
	xmEye.y = eye.y;
	xmEye.z = eye.z;

	XMFLOAT3 xmTarget;
	xmTarget.x = target.x;
	xmTarget.y = target.y;
	xmTarget.z = target.z;

	XMFLOAT3 xmUp;
	xmUp.x = up.x;
	xmUp.y = up.y;
	xmUp.z = up.z;

	XMMATRIX xmMatView = XMMatrixLookAtLH(
		XMLoadFloat3(&xmEye) , XMLoadFloat3(&xmTarget) , XMLoadFloat3(&xmUp));


	matView = MathFunc::Utility::ConvertXMMATRIXtoMatrix4(xmMatView);

	return matView;

}

#pragma region//度数法⇔弧度法の関数

//度数法を弧度法に変換する関数
float MathFunc::Utility::Deg2Rad(float Deg) {

	return Deg * MathFunc::PI / 180;

}

//弧度法を度数法に変換する関数
float MathFunc::Utility::Rad2Deg(float rad) {

	return rad / MathFunc::PI * 180;

}

#pragma endregion

//ベクトルと行列の掛け算をする関数
Vector3  MathFunc::Utility::MulVector3AndMatrix4(Vector3 vec , Matrix4 mat) {

	Vector3 ans;

	ans.x =
		mat.m[0][0] * vec.x +
		mat.m[1][0] * vec.y +
		mat.m[2][0] * vec.z +
		mat.m[3][0] * 0;

	ans.y =
		mat.m[0][1] * vec.x +
		mat.m[1][1] * vec.y +
		mat.m[2][1] * vec.z +
		mat.m[3][1] * 0;

	ans.z =
		mat.m[0][2] * vec.x +
		mat.m[1][2] * vec.y +
		mat.m[2][2] * vec.z +
		mat.m[3][2] * 0;

	return ans;

}