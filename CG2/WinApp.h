#pragma once
#include <Windows.h>



class WinApp
{
public:
	const int window_width = 1280;
	const int window_height = 720;
	RECT wrc;
	HWND hwnd;
	MSG msg;

	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	void Initialize();

};