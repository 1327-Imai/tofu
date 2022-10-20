#include <random>
#include <DirectXTex.h>

#include "DX12base.h"
#include "Model.h"
#include "WorldTransform.h"
#include "GameObject3D.h"

#include "MathFunc.h"

#include "WinApp.h"
#include "input.h"
#include "ViewProjection.h"

WinApp winApp_;

const double PI = 3.141592;

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> distPosX(-100.0 , 100.0);
std::uniform_real_distribution<float> distPosY(-50.0 , 50.0);
std::uniform_real_distribution<float> distPosZ(30.0 , 60.0);
std::uniform_real_distribution<float> distRot(0 , XMConvertToRadians(360.0f));

#pragma region//構造体

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	Vector4 color; //色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	Matrix4 mat; //3D変換行列
};

struct Object3D {

	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ワールド変換
	WorldTransform worldTransform;

};

#pragma endregion

#pragma region//関数のプロトタイプ宣言
//ウィンドウプロシーシャ
LRESULT WindowProc(HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam);

//3Dオブジェクト初期化
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device);

void UpdateObject3d(Object3D* object , ViewProjection viewProjection , XMMATRIX& matProjection);

void DrawObject3d(Object3D* object , DX12base dx12base , Model model);

void MoveObject3d(GameObject3D* object , BYTE key[256]);

void SetRandomPositionObject3d(Object3D* object);
void SetRandomRotateObject3d(Object3D* object);

#pragma endregion//関数のプロトタイプ宣言

//main関数
int WINAPI WinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPSTR , _In_ int) {

	//WindowsAPI初期化処理
	winApp_.Initialize();
	Input* input_ = new Input;

#pragma endregion//ウィンドウの生成

#pragma region//メッセージループ

	HRESULT result;

	DX12base dx12base;
	dx12base.SetWinApp(&winApp_);
	dx12base.Initialize();

#pragma region// キーボードデバイスの生成
	// DirectInputの初期化
	input_->DirectInputInit(winApp_);

	input_->DirectInputCreate(winApp_);
#pragma endregion


#pragma region//描画初期化処理

#pragma region//頂点データ

	Model model;
	model.SetDx12Base(&dx12base);
	model.LoadModel("Resources/triangle.obj");
	model.Initialize();

#pragma endregion

#pragma region//頂点シェーダー
	//頂点シェーダーファイルの読み込みとコンパイル
	ID3DBlob* vsBlob = nullptr;		//頂点シェーダーオブジェクト
	ID3DBlob* psBlob = nullptr;		//ピクセルシェーダーオブジェクト
	ID3DBlob* errorBlob = nullptr;	//エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl" ,									//シェーダーファイル名
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//インクルード可能にする
		"main" ,											//エントリーポイント名
		"vs_5_0" ,											//シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//デバッグ用設定
		0 ,
		&vsBlob ,
		&errorBlob
	);

	//シェーダーコードのエラー
	//エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region//ピクセルシェーダー
	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl" ,									//シェーダーファイル名
		nullptr ,
		D3D_COMPILE_STANDARD_FILE_INCLUDE ,					//インクルード可能にする
		"main" ,												//エントリーポイント名
		"ps_5_0" ,											//シェーダーモデル設定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION ,	//デバッグ用設定
		0 ,
		&psBlob ,
		&errorBlob
	);

	//シェーダーコードのエラー
	//エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer() ,
					errorBlob->GetBufferSize() ,
					error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz座標
			"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//法線ベクトル
			"NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
		{//uv座標
			"TEXCODE" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 ,
			D3D12_APPEND_ALIGNED_ELEMENT ,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0
		} ,
	};

#pragma region//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;			//背面化リング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;			//ポリゴン内塗りつぶし
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;	//ワイヤーフレーム表示
	pipelineDesc.RasterizerState.DepthClipEnable = true;					//深度クリッピングを有効に

	//ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA全てのチャンネルを描画

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//共通設定(アルファ値)
	blenddesc.BlendEnable = false;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;								//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//深度値フォーマット

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;								//描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1;								//1ピクセルにつき1回のサンプリング
#pragma endregion

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region//ルートパラメータ
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダーから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
#pragma endregion

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma region//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;					//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc ,
		D3D_ROOT_SIGNATURE_VERSION_1_0 ,
		&rootSigBlob ,
		&errorBlob);
	assert(SUCCEEDED(result));

	result = dx12base.GetDevice()->CreateRootSignature(
		0 ,
		rootSigBlob->GetBufferPointer() ,
		rootSigBlob->GetBufferSize() ,
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();
#pragma endregion

	//パイプラインステートの生成
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dx12base.GetDevice()->CreateGraphicsPipelineState(&pipelineDesc , IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma region//定数バッファ

	//定数バッファの生成
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
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

	//定数バッファの生成
	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

#pragma endregion

#pragma region//射影変換
	//単位行列を代入
	//constMapTransform->mat = XMMatrixIdentity();

	////平行投影変換行列の計算
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(
	//	0.0f , WINDOW_WIDTH ,
	//	WINDOW_HEIGHT , 0.0f ,
	//	0.0f , 1.0f
	//);

	//透視投影変換行列の計算
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp_.window_width / winApp_.window_height ,
		0.1f , 1000.0f
	);

#pragma endregion

#pragma region//ビュー変換行列
	ViewProjection viewProjection_;
	viewProjection_.Initialize();
	viewProjection_.UpdateView();

	float angle = 0.0f;

#pragma endregion

#pragma region//ワールド変換行列

	//3Dオブジェクトの数
	const size_t kObjectCount = 50;

	Object3D object3ds[kObjectCount];

	GameObject3D gameObject;
	GameObject3D gameObject2;

	for (int i = 0; i < _countof(object3ds); i++) {

		//初期化
		InitializeObject3d(&object3ds[i] , dx12base.GetDevice());
		object3ds[i].worldTransform.initialize();

		//ここから下は親子構造のサンプル
		//先頭以外なら
		if (i > 0) {
			////ひとつ前のオブジェクトを親オブジェクトとする
			//object3ds[i].worldTransform.parent = &object3ds[i - 1].worldTransform;
			////親オブジェクトの9割の大きさ
			//object3ds[i].worldTransform.scale = {0.9f , 0.9f , 0.9f};
			////親オブジェクトに対してZ軸まわりに30度回転
			//object3ds[i].worldTransform.rotation = {0.0f , 0.0f , XMConvertToRadians(30.0f)};

			////親オブジェクトに対してZ軸方向に-8.0ずらす
			//object3ds[i].worldTransform.translation = {0.0f , 0.0f , -8.0f};

			//ランダムに配置する
			SetRandomPositionObject3d(&object3ds[i]);
			SetRandomRotateObject3d(&object3ds[i]);
		}

		//object3ds->Update();
	}

	gameObject.SetDX12Base(&dx12base);
	gameObject.Initialize();
	gameObject.SetViewProjection(&viewProjection_);
	gameObject.SetMatProjection(&matProjection);

	gameObject2.SetDX12Base(&dx12base);
	gameObject2.Initialize();
	gameObject2.SetViewProjection(&viewProjection_);
	gameObject2.SetMatProjection(&matProjection);

#pragma endregion

#pragma region//テクスチャ
	//画像イメージデータの作成
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/texture.jpg" ,
		WIC_FLAGS_NONE ,
		&metadata ,
		scratchImg
	);

	//画像イメージデータの作成
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/reimu.png" ,
		WIC_FLAGS_NONE ,
		&metadata2 ,
		scratchImg2
	);


	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages() ,
		scratchImg.GetImageCount() ,
		scratchImg.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	ScratchImage mipChain2{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg2.GetImages() ,
		scratchImg2.GetImageCount() ,
		scratchImg2.GetMetadata() ,
		TEX_FILTER_DEFAULT ,
		0 ,
		mipChain2
	);
	if (SUCCEEDED(result)) {
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;	//幅
	textureResouceDesc.Height = (UINT)metadata.height;	//高さ
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc2{};
	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc2.Format = metadata2.format;
	textureResouceDesc2.Width = metadata2.width;	//幅
	textureResouceDesc2.Height = (UINT)metadata2.height;	//高さ
	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResouceDesc2.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff = nullptr;
	result = dx12base.GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff2 = nullptr;
	result = dx12base.GetDevice()->CreateCommittedResource(
		&textureHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&textureResouceDesc2 ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&texBuff2)
	);
	assert(SUCCEEDED(result));

	//テクスチャバッファにデータ転送
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img->pixels ,			//元データアドレス
			(UINT)img->rowPitch ,	//1ラインサイズ
			(UINT)img->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	//全ミップマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img2 = scratchImg2.GetImage(i , 0 , 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i ,
			nullptr ,				//全領域へコピー
			img2->pixels ,			//元データアドレス
			(UINT)img2->rowPitch ,	//1ラインサイズ
			(UINT)img2->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

#pragma endregion

#pragma region//シェーダーリソースビュー
	//デスクリプタヒープの生成
	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = dx12base.GetDevice()->CreateDescriptorHeap(
		&srvHeapDesc ,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//デスクリプタハンドル
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//シェーダーリソースビューの作成
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = model.GetResDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = model.GetResDesc().MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12base.GetDevice()->CreateShaderResourceView(texBuff.Get() , &srvDesc , srvHandle);

	//二枚目
	//SRVヒープの先頭ハンドルを取得
	UINT incremantSize = dx12base.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incremantSize;

	//シェーダーリソースビューの作成
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};	//設定構造体
	srvDesc2.Format = textureResouceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dx12base.GetDevice()->CreateShaderResourceView(texBuff2.Get() , &srvDesc2 , srvHandle);

#pragma endregion

#pragma endregion//描画初期化処理

#pragma region//ゲームループ
	while (true) {

#pragma region//ウィンドウメッセージ処理
		//メッセージがある?
		if (PeekMessage(&winApp_.msg , nullptr , 0 , 0 , PM_REMOVE)) {
			TranslateMessage(&winApp_.msg);	//キー入力メッセージの処理
			DispatchMessage(&winApp_.msg);	//プロシーシャにメッセージを送る
		}

		//×ボタンで終了メッセージが来たらゲームループを抜ける
		if (winApp_.msg.message == WM_QUIT) {
			break;
		}
#pragma endregion//ウィンドウメッセージ処理

#pragma region//DirectX毎フレーム処理

		// キーボード情報の取得開始
		input_->Update();

#pragma region//更新処理

		//ビュー変換
		//いずれかのキーを押していたら
		if (input_->PushKey(DIK_D) || input_->PushKey(DIK_A)) {

			//押したキーに応じてangleを増減させる
			if (input_->PushKey(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			if (input_->PushKey(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			//angleラジアンだけY軸周りに回転。半径は-100
			viewProjection_.eye.x = -100 * sinf(angle);
			viewProjection_.eye.z = -100 * cosf(angle);

			//ビュー変換行列を作り直す
			viewProjection_.UpdateView();

		}

		//MoveObject3d(&object3ds[0] , input_->key);

		for (int i = 0; i < _countof(object3ds); i++) {
			UpdateObject3d(&object3ds[i] , viewProjection_ , matProjection);
		}

		MoveObject3d(&gameObject , input_->key);

		gameObject.Update();
		gameObject2.Update();

#pragma endregion//更新処理

		//描画前処理
		dx12base.PreDraw();

#pragma region//描画処理

		//パイプラインステートとルートシグネチャの設定コマンド
		dx12base.GetCmdList()->SetPipelineState(pipelineState.Get());
		dx12base.GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());

		//プリミティブ形状の設定コマンド
		dx12base.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//model.Draw();

		//頂点バッファ―ビューをセットするコマンド
		dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0 , constBuffMaterial->GetGPUVirtualAddress());

		//SRVヒープの設定コマンド
		dx12base.GetCmdList()->SetDescriptorHeaps(1 , srvHeap.GetAddressOf());

		//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		if (input_->PushKey(DIK_SPACE)) {
			//二枚目を指し示すように差し込む
			srvGpuHandle.ptr += incremantSize;
		}

		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		dx12base.GetCmdList()->SetGraphicsRootDescriptorTable(1 , srvGpuHandle);

		for (int i = 0; i < _countof(object3ds); i++) {
			//DrawObject3d(&object3ds[i] , dx12base , model);
		}

		gameObject.Draw();
		gameObject2.Draw();

#pragma endregion

		//描画後処理
		dx12base.PostDraw();

#pragma endregion//DirectX毎フレーム処理

	}
#pragma endregion//ゲームループ

	//ID3D12DebugDevice* debugInterface;

	//if (SUCCEEDED(device.Get()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}

	//ウィンドウクラス登録解除
	UnregisterClass(winApp_.w.lpszClassName , winApp_.w.hInstance);
#pragma endregion//メッセージループ

	return 0;
}


#pragma region//関数の定義

//3Dオブジェクト初期化
void InitializeObject3d(Object3D* object , ComPtr<ID3D12Device> device) {

	HRESULT result;

	//定数バッファの生成
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//定数バッファの生成
	result = device.Get()->CreateCommittedResource(
		&cbHeapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&cbResourceDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&object->constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = object->constBuffTransform->Map(0 , nullptr , (void**)&object->constMapTransform); // マッピング
	assert(SUCCEEDED(result));

}

void UpdateObject3d(Object3D* object , ViewProjection viewProjection , XMMATRIX& matProjection) {

	object->worldTransform.UpdateMatWorld();

	//定数バッファへデータ転送
	object->constMapTransform->mat = object->worldTransform.matWorld;
	object->constMapTransform->mat *= viewProjection.matView;
	object->constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(matProjection);

}

void DrawObject3d(Object3D* object , DX12base dx12base , Model model) {

	//定数バッファビュー(CBV)の設定コマンド
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2 , object->constBuffTransform->GetGPUVirtualAddress());

	model.Draw();
}

void MoveObject3d(GameObject3D* object , BYTE key[256]) {
	if (key[DIK_UP] || key[DIK_DOWN] || key[DIK_RIGHT] || key[DIK_LEFT]) {

		if (key[DIK_UP]) {
			object->worldTransform.translation.y += 1.0f;
		}
		if (key[DIK_DOWN]) {
			object->worldTransform.translation.y -= 1.0f;
		}
		if (key[DIK_RIGHT]) {
			object->worldTransform.translation.x += 1.0f;
		}
		if (key[DIK_LEFT]) {
			object->worldTransform.translation.x -= 1.0f;
		}
	}
}

void SetRandomPositionObject3d(Object3D* object) {
	object->worldTransform.translation.x = distPosX(engine);
	object->worldTransform.translation.y = distPosY(engine);
	object->worldTransform.translation.z = distPosZ(engine);
}

void SetRandomRotateObject3d(Object3D* object) {
	object->worldTransform.rotation.x = distRot(engine);
	object->worldTransform.rotation.y = distRot(engine);
	object->worldTransform.rotation.z = distRot(engine);
}

#pragma endregion//関数の定義