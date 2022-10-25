#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputVersion
#include <dinput.h>
#include "WinApp.h"
#include<wrl.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Singleton.h"

class Input :public Singleton<Input>
{
	friend Singleton<Input>;

public:
	IDirectInputDevice8* keyboard;
	IDirectInput8* directInput;

	unsigned char key[256] = {};
	unsigned char oldkey[256] = {};

	bool PushKey(unsigned char keys);
	bool TriggerKey(unsigned char keys);
	bool ReleaseKey(unsigned char keys);

	void DirectInputInit(WinApp winApp_);
	void DirectInputCreate(WinApp winApp_);

	void Update();

private:
	Input();
	~Input();

};
