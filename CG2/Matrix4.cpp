#include "Matrix4.h"
//コンストラクタ
Matrix4::Matrix4() {

	SetIdentityMatrix();

}

Matrix4::Matrix4(
	float m00 , float m01 , float m02 , float m03 ,
	float m10 , float m11 , float m12 , float m13 ,
	float m20 , float m21 , float m22 , float m23 ,
	float m30 , float m31 , float m32 , float m33
) {

	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;

}

void Matrix4::SetIdentityMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0.0f;
		}
	}

	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}

Matrix4 Matrix4::operator+=(const Matrix4& m) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->m[i][j] += m.m[i][j];
		}
	}

	return *this;
}

Matrix4 Matrix4::operator-=(const Matrix4& m) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->m[i][j] -= m.m[i][j];
		}
	}

	return *this;
}

Matrix4 Matrix4::operator*=(float s) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->m[i][j] *= s;
		}
	}

	return *this;
}

Matrix4 Matrix4::operator*=(const Matrix4& m) {

	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += this->m[i][k] * m.m[k][j];
			}
		}
	}

	*this = result;

	return *this;
}