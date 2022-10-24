#include "GameObject3D.h"

class ViewProjection {
public:
	Matrix4 matView;
};

//メンバ関数
void GameObject3D::PreLoadModel(const char* modelFileName) {
	this->modelFileName = modelFileName;
}

//メンバ関数
void GameObject3D::PreLoadTexture(const wchar_t* textureFileName) {
	this->textureFileName = textureFileName;
}

void GameObject3D::Initialize() {

	InitializeConstMapTransform();
	InitializeConstMapMaterial();

	//ワールド変換の初期化
	worldTransform.initialize();

	//モデルの初期化
	model.LoadModel(modelFileName);
	model.Initialize();

	textrue.LoadTexture(textureFileName);
	textrue.SetModel(&model);
	textrue.CreateSRV();
}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

	//定数バッファへデータ転送
	constMapTransform->mat = worldTransform.matWorld;
	constMapTransform->mat *= viewProjection->matView;
	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);

}

void GameObject3D::Draw() {

	//頂点バッファ―ビューをセットするコマンド
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

	//定数バッファビュー(CBV)の設定コマンド
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	textrue.Draw();

	model.Draw();

}

void GameObject3D::InitializeConstMapTransform() {
	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	cbTransformHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	cbTransformResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbTransformResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbTransformResourceDesc.Height = 1;
	cbTransformResourceDesc.DepthOrArraySize = 1;
	cbTransformResourceDesc.MipLevels = 1;
	cbTransformResourceDesc.SampleDesc.Count = 1;
	cbTransformResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbTransformHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbTransformResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));

}

void GameObject3D::InitializeConstMapMaterial() {

	HRESULT result;

	//定数バッファの生成
//ヒープ設定
	cbMaterialHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};
	cbMaterialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMaterialResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbMaterialResourceDesc.Height = 1;
	cbMaterialResourceDesc.DepthOrArraySize = 1;
	cbMaterialResourceDesc.MipLevels = 1;
	cbMaterialResourceDesc.SampleDesc.Count = 1;
	cbMaterialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbMaterialHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbMaterialResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0 , nullptr , (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	//定数バッファへのデータ転送
	//値を書き込むと自動的に転送される
	constMapMaterial->color = Vector4(1.0f , 1.0f , 1.0f , 0.5f);

}


//アクセッサ
void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
	this->viewProjection = viewProjection;
}

void GameObject3D::SetMatProjection(XMMATRIX* matProjection) {
	this->matProjection = matProjection;
}