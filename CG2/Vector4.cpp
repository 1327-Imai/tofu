#include "Vector4.h"

Vector4::Vector4() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Vector4::Vector4(float x , float y , float z , float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}