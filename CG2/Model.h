#pragma once
#include <d3d12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXmath.h>
#include <d3dcompiler.h>

#include<wrl.h>

#include "DX12base.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")

#include <string.h>

using namespace Microsoft::WRL;
using namespace DirectX;

//class DX12base;

class Model{
public:
	//メンバ関数
	void LoadModel();
	void LoadModel(const char* fileName);

	void Initialize();

	void Draw();

public:
	//アクセッサ
	D3D12_RESOURCE_DESC GetResDesc();

	D3D12_VERTEX_BUFFER_VIEW GetVbView();

	D3D12_INDEX_BUFFER_VIEW GetIbView();

private:
	//構造体
	struct Vertex {
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;	//uv座標
	};

	//メンバ変数
	//result
	HRESULT result;

	//dierectX12の基礎設定
	DX12base& dx12base = DX12base::GetInstance();

	//モデルのファイル形式
	char fileType[5]={0};

	// 頂点データ配列
	std::vector<Vertex> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;

	//頂点データ全体のサイズ
	UINT sizeVB;

	//インデックスデータ全体のサイズ
	UINT sizeIB;

	//リソース設定
	D3D12_RESOURCE_DESC resDesc;

	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;

	//GPU上のバッファのマップ
	Vertex* vertMap = nullptr;

	//インデックスバッファのマップ
	uint16_t* indexMap = nullptr;

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView;
};

