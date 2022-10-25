#pragma once
#include "ErrorException.h"

#pragma comment(lib, "winmm.lib")

class FPS {
  private:
	// CPU���g��
	LARGE_INTEGER cpuClock{};
	//�v���J�n����
	LARGE_INTEGER timeStart{};
	//�v���I������
	LARGE_INTEGER timeEnd{};
	//�Œ肷�鎞��
	float frameTime = 1 / 60.0f;
	// FPS�l
	float fps;

  public:
	/// <summary>
	/// FPS���䏉����
	/// </summary>
	void FpsControlBegin();

	/// <summary>
	/// FPS����
	/// </summary>
	void FpsControlEnd();

	/// <summary>
	/// �t���[�����[�g��ݒ�
	/// </summary>
	/// <param name="fps_">�t���[�����[�g</param>
	void SetFrameRate(float fps_);

	/// <summary>
	/// �t���[�����[�g���擾
	/// </summary>
	/// <returns>�t���[�����[�g</returns>
	float GetFrameRate();
};