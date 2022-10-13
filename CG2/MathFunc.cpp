#include "MathFunc.h"
//�X�P�[�����O�s���ݒ肷��֐�
void MathFunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matScale;

}

//��]�s���ݒ肷��֐�
void MathFunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	//��]�s���錾
	//Z����]
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X����]
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , -(float)sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y����]
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//���s�ړ��s��̐ݒ������֐�
void MathFunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//���[���h�ϊ��s��𐶐�����֐�
//�X�P�[�����O�s��𐶐�����֐�
Matrix4 MathFunc::Affine::CreateMatScale(Vector3 scale) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	return matScale;

}

//��]�s��𐶐�����֐�
Matrix4 MathFunc::Affine::CreateMatRotation(Vector3 rotation) {

	//��]�s���錾
	Matrix4 matRot;
	matRot.SetIdentityMatrix();

	//Z����]
	Matrix4 matRotZ = {
		(float)cos(rotation.z) , (float)sin(rotation.z) , 0 , 0 ,
		(float)-sin(rotation.z) , (float)cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X����]
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , (float)cos(rotation.x) , (float)sin(rotation.x) , 0 ,
		0 , (float)-sin(rotation.x) , (float)cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y����]
	Matrix4 matRotY = {
		(float)cos(rotation.y) , 0 , (float)-sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		(float)sin(rotation.y) , 0 , (float)cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}

//���s�ړ��s��̐���������֐�
Matrix4 MathFunc::Affine::CreateMatTranslation(Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	return matTranslation;
}

#pragma endregion

#pragma region//�x���@�̌ʓx�@�̊֐�

//�x���@���ʓx�@�ɕϊ�����֐�
float MathFunc::Utility::Deg2Rad(float Deg) {

	return Deg * MathFunc::PI / 180;

}

//�ʓx�@��x���@�ɕϊ�����֐�
float MathFunc::Utility::Rad2Deg(float rad) {

	return rad / MathFunc::PI * 180;

}

#pragma endregion