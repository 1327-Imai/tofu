#include "GameScene.h"
#include <cassert>



GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete dx12base;
	delete input;
	delete gameObject;
	delete gameObject2;
	delete viewProjection_;
	delete winApp_;

}

void GameScene::Initialize(DX12base* DX12base, Input* input, WinApp* winApp)
{
	// nullptrチェック
	assert(DX12base);
	assert(input);

	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		1280 / 790,
		0.1f, 1000.0f
	);
	
	viewProjection_->Initialize();
	viewProjection_->UpdateView();


	//
	gameObject->SetDX12Base(dx12base);
	gameObject->PreLoadTexture(L"Resources/texture.jpg");
	gameObject->Initialize();
	gameObject->SetViewProjection(viewProjection_);
	gameObject->SetMatProjection(&matProjection);

	

	gameObject2->SetDX12Base(dx12base);
	gameObject2->Initialize();
	gameObject2->SetViewProjection(viewProjection_);
	gameObject2->SetMatProjection(&matProjection);
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}
