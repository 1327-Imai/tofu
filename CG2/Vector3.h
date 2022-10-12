#pragma once
class Vector3 {
public:
	//コンストラクタ
	Vector3();	//零ベクトルとする
	Vector3(float x,float y,float z);	//xyz成分を指定しての生成

	//メンバ関数
	float length() const;
	Vector3& nomalize();
	float dot(const Vector3& v) const;
	Vector3 cross(const Vector3& v) const;

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3 operator+=(const Vector3& v);
	Vector3 operator-=(const Vector3& v);
	Vector3 operator*=(float s);
	Vector3 operator/=(float s);

	//メンバ変数
public:
	float x;	//x成分
	float y;	//y成分
	float z;	//z成分

};
//3項演算子オーバーロード
//いろんな引数(引数の型と順序)のパターンに対応するために、以下のように準備している
const Vector3& operator+(const Vector3& v1 , const Vector3& v2);
const Vector3& operator-(const Vector3& v1 , const Vector3& v2);
const Vector3& operator*(const Vector3& v , float s);
const Vector3& operator*(float s , const Vector3& v);
const Vector3& operator/(const Vector3& v , float s);

