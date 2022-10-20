#include "GameObject3D.h"

//前方宣言の定義
class DX12base {
public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	ComPtr<ID3D12Device> GetDevice();
};
class ViewProjection {
public:
	Matrix4 matView;
};

//メンバ関数
void GameObject3D::Initialize() {

	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dx12base->GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0 , nullptr , (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));

	//ワールド変換の初期化
	worldTransform.initialize();

	//モデルの初期化
	model.LoadModel();
	model.Initialize();

}

void GameObject3D::Update() {

	worldTransform.UpdateMatWorld();

	//定数バッファへデータ転送
	constMapTransform->mat = worldTransform.matWorld;
	constMapTransform->mat *= viewProjection->matView;
	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);

}

void GameObject3D::Draw() {

	//定数バッファビュー(CBV)の設定コマンド
	dx12base->GetCmdList()->SetGraphicsRootConstantBufferView(2 , constBuffTransform->GetGPUVirtualAddress());

	model.Draw();

}

//アクセッサ
void GameObject3D::SetDX12Base(DX12base* dx12base) {
	this->dx12base = dx12base;
	model.SetDx12Base(dx12base);
}

void GameObject3D::SetViewProjection(ViewProjection* viewProjection) {
	this->viewProjection = viewProjection;
}

void GameObject3D::SetMatProjection(XMMATRIX* matProjection) {
	this->matProjection = matProjection;
}