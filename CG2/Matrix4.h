#pragma once

class Matrix4 {
public:
	//コンストラクタ
	Matrix4();
	Matrix4(
		float m00 , float m01 , float m02 , float m03 ,
		float m10 , float m11 , float m12 , float m13 ,
		float m20 , float m21 , float m22 , float m23 ,
		float m30 , float m31 , float m32 , float m33
		);

	//メンバ関数
	//単位行列を代入する
	void SetIdentityMatrix();

	//代入演算子オーバーロード
	Matrix4 operator+=(const Matrix4& m);
	Matrix4 operator-=(const Matrix4& m);
	Matrix4 operator*=(float s);
	Matrix4 operator*=(const Matrix4& m);

	//メンバ変数
	float m[4][4];

};

