#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Texture.h"
#include "DX12base.h"

#include "MathFunc.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


//前方宣言
//class DX12base;
class ViewProjection;

class GameObject3D {

public:
	//メンバ変数
	void PreLoadModel(const char* modelFileName);
	void PreLoadTexture(const wchar_t* textureFileName);

	void Initialize();

	void Update();

	void Draw();

	//アクセッサ
	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);

private:
	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	//構造体
private:
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


private:
	//モデル
	Model model;
	//モデルのファイル名
	const char* modelFileName = nullptr;

	//テクスチャ
	Texture textrue;
	//モデルのファイル名
	const wchar_t* textureFileName = nullptr;

	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbTransformHeapProp;
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};

	//リソース設定
	D3D12_RESOURCE_DESC cbTransformResourceDesc;
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	XMMATRIX* matProjection;

	//DirectX基礎部分
	DX12base& dx12base = DX12base::GetInstance();
	//ビュープロジェクション
	ViewProjection* viewProjection;

};

