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

	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	void Initialize();

};