#include "Vector3.h"
#include <cmath>

Vector3::Vector3() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x , float y , float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::nomalize() {

	float len = length();
	if (len != 0) {
		return *this = *this / len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const {
	return Vector3(y * v.z - z * v.y ,
				   z * v.x - x * v.z ,
				   x * v.y - y * v.x);
}

Vector3 Vector3::operator+()const {
	return *this;
}

Vector3 Vector3::operator-()const {
	return Vector3(-x , -y , -z);
}

Vector3 Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector3& operator+(const Vector3& v1 , const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3& operator-(const Vector3& v1 , const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3& operator*(const Vector3& v , float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3& operator*(float s , const Vector3& v) {
	return v * s;
}

const Vector3& operator/(const Vector3& v , float s) {
	Vector3 temp(v);
	return temp /= s;
}