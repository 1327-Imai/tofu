#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
using namespace DirectX;

class ViewProjection
{
public:
	XMMATRIX matView;
	XMFLOAT3 eye;	//���_���W
	XMFLOAT3 target;	//�����_���W
	XMFLOAT3 up;		//������x�N�g��

public:
	void Initialize();
	void UpdateView();
};

