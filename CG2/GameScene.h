#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"

#include "Audio.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "Player.h"
#include "Map.h"

class GameScene {

public: // �����o�֐�

	//�R���X�g���N�^
	GameScene();

	/// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(WinApp* winApp);

	//���t���[������
	void Update();

	//�`�揈��
	void Draw();

private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	SoundManager soundManager_;

	ViewProjection viewProjection_;

	//�����ǂݍ���
	SoundData soundData1 = soundManager_.SoundLoadWave("Resources/Alarm01.wav");

	bool isPlayingBGM = false;

	Player* player = nullptr;

	Map* map = nullptr;

};