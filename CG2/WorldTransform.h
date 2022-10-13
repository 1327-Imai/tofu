#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "MathFunc.h"

class WorldTransform {
public:
	//�����o�֐�
	void initialize();

	//���[���h�ϊ��s��̍X�V
	void UpdateMatWorld();

	//�����o�ϐ�
	//�X�P�[��
	Vector3 scale;
	//XYZ����]
	Vector3 rotation;
	//���[�J�����W
	Vector3 translation;
	//���[���h�ϊ��s��
	Matrix4 matWorld;
	//�e�I�u�W�F�N�g�̃��[���h�ϊ��̃|�C���^
	WorldTransform* parent = nullptr;

};

