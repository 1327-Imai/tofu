#pragma once
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <cassert>
#include <vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "WinApp.h"
#include "DX12base.h"

#include <DirectXTex.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Model;
//class DX12base;

class Texture {

	//メンバ関数
public:
	void LoadTexture(const wchar_t* fileName);

	void CreateSRV();

	void Draw();

	//アクセッサ
	void SetModel(Model* model);

	//メンバ変数
private:
	//画像イメージデータの作成
	TexMetadata metadata;
	ScratchImage scratchImg;

	//ミップマップ
	ScratchImage mipChain;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc;

	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff = nullptr;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	//設定をもとにSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//DirectX基礎部分
	DX12base& dx12base =DX12base::GetInstance();

	//モデル
	Model* model;

};