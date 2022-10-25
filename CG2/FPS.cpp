#include "FPS.h"

void FPS::FpsControlBegin() {
	//���g���擾
	QueryPerformanceFrequency(&cpuClock);
	//�v���J�n���Ԃ̏�����
	QueryPerformanceCounter(&timeStart);
}

void FPS::FpsControlEnd() {
	//���̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);
	//�o�ߎ���
	float elapsedFrame = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) /
	                     static_cast<float>(cpuClock.QuadPart);
	//�]�T������Ƃ��͑҂�
	if (elapsedFrame < frameTime) {
		// sleep����
		DWORD sleepTime = static_cast<DWORD>((frameTime - elapsedFrame) * 1000.0f);
		timeBeginPeriod(1);
		//�Q��
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	fps = 1 / elapsedFrame;
}

void FPS::SetFrameRate(float fps_) { frameTime = 1.0f / fps_; }

float FPS::GetFrameRate() { return fps; }
