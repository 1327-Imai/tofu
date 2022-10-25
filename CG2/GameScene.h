#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"

#include "Audio.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class GameScene {

public: // �����o�֐�

	//�R���X�g���N�^
	GameScene();

	/// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(DX12base* dx12base , Input* input , WinApp* winApp);

	//���t���[������
	void Update();

	//�`�揈��
	void Draw();

private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DX12base* dx12base_ = nullptr;
	Input* input_ = nullptr;
	XMMATRIX matProjection_;
	SoundManager soundManager_;

	ViewProjection viewProjection_;
	float angle = 0.0f;

	GameObject3D* gameObject = nullptr;
	GameObject3D* gameObject2 = nullptr;

	//�����ǂݍ���
	SoundData soundData1 = soundManager_.SoundLoadWave("Resources/Alarm01.wav");

	bool isPlayingBGM = false;

	//�V�[���Ǘ�
	enum class Scene
	{
		Title,//�^�C�g��
		Stage,//�X�e�[�W
		Pose,//�|�[�Y
		Clear,//�N���A
		Over,//�Q�[���I�[�o�[
	};

	Scene scene_ = Scene::Title;

};