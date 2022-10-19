#include "WorldTransform.h"

void WorldTransform::initialize() {

	scale = {1 , 1 , 1};
	rotation = {0 , 0 , 0};
	translation = {0 , 0 , 0};
	matWorld.SetIdentityMatrix();

	UpdateMatWorld();

}

//���[���h�ϊ��s��̍X�V
void WorldTransform::UpdateMatWorld() {

	//���[���h�ϊ��s���p��
	Matrix4 affineMat;
	affineMat.SetIdentityMatrix();

	//���[���h�ϊ��s��ɃX�P�[�����O,��],���s�ړ��̍s�������
	MathFunc::Affine::SetMatScale(affineMat , scale);
	MathFunc::Affine::SetMatRotation(affineMat , rotation);
	MathFunc::Affine::SetMatTranslation(affineMat , translation);

	//���[���h�s��ɒP�ʍs�����
	matWorld.SetIdentityMatrix();

	//�s��̌v�Z
	matWorld *= affineMat;

	//�����e������ꍇ
	if (parent) {
		//�e�̃��[���h�s��Ƃ̌v�Z���s��
		matWorld *= parent->matWorld;
	}

}