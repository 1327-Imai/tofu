#pragma once
class Vector2 {
	//�R���X�g���N�^
	Vector2();					//��x�N�g���Ƃ��Đ���
	Vector2(float x, float y);	//x����,y�������w�肵�Ă̐���

	//�����o�֐�
	float length() const;
	Vector2& nomalize();
	float dot(const Vector2& v) const;
	float cross(const Vector2& v) const;

	//�P�����Z�q�I�[�o�[���[�h
	Vector2 operator+() const;
	Vector2 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector2 operator+=(const Vector2& v);
	Vector2 operator-=(const Vector2& v);
	Vector2 operator*=(float s);
	Vector2 operator/=(float s);

	//�����o�ϐ�
public:
	float x;
	float y;

};
//2�����Z�q�I�[�o�[���[�h
//�����Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߂ɁA�ȉ��̂悤�ɏ������Ă���
const Vector2& operator+(const Vector2& v1, const Vector2& v2);
const Vector2& operator-(const Vector2& v1, const Vector2& v2);
const Vector2& operator*(const Vector2& v, float s);
const Vector2& operator*(float s, const Vector2& v);
const Vector2& operator/(const Vector2& v, float s);

