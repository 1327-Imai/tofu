#include "Input.h"
#include <cassert>

static HRESULT result;

Input::Input()
{
	keyboard = nullptr;
	directInput = nullptr;
}

Input::~Input()
{
}

bool Input::PushKey(unsigned char keys)
{
	return key[keys];
}

bool Input::TriggerKey(unsigned char keys)
{
	return key[keys] && !oldkey[keys];
}

bool Input::ReleaseKey(unsigned char keys)
{
	return !key[keys] && oldkey[keys];;
}

void Input::DirectInputInit(WinApp winApp_)
{
	result = DirectInput8Create(
		winApp_.w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
}

void Input::DirectInputCreate(WinApp winApp_)
{
	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	result = keyboard->SetCooperativeLevel(
		winApp_.hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);
}