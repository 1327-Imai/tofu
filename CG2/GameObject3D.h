#pragma once
#include "WorldTransform.h"
#include "Model.h"

#include "MathFunc.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


//前方宣言
class DX12base;
class ViewProjection;

class GameObject3D {

public:
	//メンバ変数
	void Initialize();

	void Update();

	void Draw();

	//アクセッサ
	void SetDX12Base(DX12base* dx12base);
	//void LoadModel(Model* model);
	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);

private:
	//構造体
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		Vector4 color; //色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		Matrix4 mat; //3D変換行列
	};

	//メンバ変数
public:
	//ワールド変換
	WorldTransform worldTransform;

	//モデルのファイル名
	char* modelFileName = nullptr;

private:
	//モデル
	Model model;

	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp;

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	XMMATRIX* matProjection;

	//DirectX基礎部分
	DX12base* dx12base;
	//ビュープロジェクション
	ViewProjection* viewProjection;

};

