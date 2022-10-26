#include "Model.h"

//メンバ関数
void Model::LoadModel() {

	//頂点データ
	vertices = {
		//前
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-1.0f , 1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{1.0f , -1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{1.0f , 1.0f , -1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//後
		{{-1.0f , -1.0f , 1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-1.0f , 1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{1.0f , -1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//左
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{-1.0f , -1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-1.0f , 1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{-1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//右
		{{1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{1.0f , -1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{1.0f , 1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//上
		{{-1.0f , 1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{1.0f , 1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-1.0f , 1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3
		//下
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//左下 インデックス0
		{{1.0f , -1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//左上 インデックス1
		{{-1.0f , -1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//右下 インデックス2
		{{1.0f , -1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//右上 インデックス3

	};

	//インデックスデータ
	indices = {
		//前
		0 , 1 , 2 , //三角形1つ目
		1 , 3 , 2 , //三角形2つ目
		//後ろ
		4 , 6 , 5 ,
		5 , 6 , 7 ,
		//左
		8 , 9 , 10 ,
		9 , 11 , 10 ,
		//右
		12 , 14 , 13 ,
		13 , 14 , 15 ,
		//上
		16 , 18 , 17 ,
		17 , 18 , 19 ,
		//下
		20 , 21 , 22 ,
		21 , 23 , 22 ,
	};

}

void Model::LoadModel(const char* fileName) {

	if (fileName) {

		//開きたいファイルの拡張子を取得
		size_t fileNameLen = strlen(fileName);
		strcpy_s(fileType , fileName + fileNameLen - 4);


#pragma region//.objファイルの場合
		if (strcmp(fileType , ".obj") == 0) {

				////ファイルストリーム
				//std::fstream file;
				////.objファイルを開く
				//file.open(fileName);
				////ファイルオープン失敗をチェック
				//if (file.fail()) {
				//	assert(0);
				//}

				//std::vector<XMFLOAT3>positions;	//頂点座標
				//std::vector<XMFLOAT3>normals;	//法線ベクトル
				//std::vector<XMFLOAT2>texcoad;	//テクスチャUV
				////1行ずつ読み込む
				//std::string line;
				//while (getline(file , line)) {

				//	//1行分の文字列をストリームに変換して解析しやすくする
				//	std::istringstream line_stream(line);

				//	//半角スペース区切りで行の先頭文字を取得
				//	std::string key;
				//	getline(line_stream , key , ' ');

				//	//先頭文字列がvなら頂点座標
				//	if (key == "v") {

				//		//X,Y,Z座標読み込み
				//		XMFLOAT3 position{};
				//		line_stream >> position.x;
				//		line_stream >> position.y;
				//		line_stream >> position.z;

				//		//座標データに追加
				//		positions.emplace_back(position);

				//		//頂点データに追加
				//		Vertex vertex{};
				//		vertex.pos = position;
				//		vertices.emplace_back(vertex);

				//	}

				//	//先頭文字列がfならポリゴン(三角形)
				//	if (key == "f") {

				//		//半角スペース区切りで行の続きを読み込む
				//		std::string index_string;
				//		while (getline(line_stream , index_string , ' ')) {

				//			//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				//			std::istringstream index_stream(index_string);
				//			unsigned short indexPosition;
				//			index_stream >> indexPosition;

				//			//頂点インデックスに追加
				//			indices.emplace_back(indexPosition - 1);

				//		}

				//	}

				//}
				//file.close();

			//ファイルストリーム
			std::fstream file;
			//.objファイルを開く
			file.open(fileName);
			//ファイルオープン失敗をチェック
			if (file.fail()) {
				assert(0);
			}

			std::vector<XMFLOAT3>positions;	//頂点座標
			std::vector<XMFLOAT3>normals;	//法線ベクトル
			std::vector<XMFLOAT2>texcoords;	//テクスチャUV
			//1行ずつ読み込む
			std::string line;
			while (getline(file , line)) {

				//1行分の文字列をストリームに変換して解析しやすくする
				std::istringstream line_stream(line);

				//半角スペース区切りで行の先頭文字を取得
				std::string key;
				getline(line_stream , key , ' ');

				//先頭文字列がvなら頂点座標
				if (key == "v") {

					//X,Y,Z座標読み込み
					XMFLOAT3 position{};
					line_stream >> position.x;
					line_stream >> position.y;
					line_stream >> position.z;

					//座標データに追加
					positions.emplace_back(position);

				}

				//先頭文字列がvtならテクスチャ
				if (key == "vt") {
					//U,V成分読み込み
					XMFLOAT2 texcoord{};
					line_stream >> texcoord.x;
					line_stream >> texcoord.y;

					//V方向反転
					texcoord.y = 1.0f - texcoord.y;

					//テクスチャ座標データに追加
					texcoords.emplace_back(texcoord);

				}

				if (key == "vn") {
					//X,Y,Z成分読み込み
					XMFLOAT3 normal{};
					line_stream >> normal.x;
					line_stream >> normal.y;
					line_stream >> normal.z;

					//法線ベクトルデータに追加
					normals.emplace_back(normal);

				}

				//先頭文字列がfならポリゴン(三角形)
				if (key == "f") {

					//半角スペース区切りで行の続きを読み込む
					std::string index_string;
					while (getline(line_stream , index_string , ' ')) {

						//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
						std::istringstream index_stream(index_string);
						unsigned short indexPosition , indexNormal , indexTexcoord;
						index_stream >> indexPosition;
						index_stream.seekg(1 , std::ios_base::cur);
						index_stream >> indexTexcoord;
						index_stream.seekg(1 , std::ios_base::cur);
						index_stream >> indexNormal;

						Vertex vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = texcoords[indexTexcoord - 1];
						vertices.emplace_back(vertex);

						//インデックスデータの追加
						indices.emplace_back((unsigned short)indices.size());
					}
				}
			}
			file.close();
		}
#pragma endregion
		else {
			//対応していないモデルはエラーを吐く
			assert(nullptr);
		}
	}
	else {
		LoadModel();
	}
}

void Model::Initialize() {

	//法線の計算
	for (int i = 0; i < indices.size() / 3; i++) {

		//三角形のインデックスを取り出して一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1 , p0);
		XMVECTOR v2 = XMVectorSubtract(p2 , p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1 , v2);

		//正規化
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal , normal);
		XMStoreFloat3(&vertices[index1].normal , normal);
		XMStoreFloat3(&vertices[index2].normal , normal);

	}


	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};		//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//インデックスバッファの生成
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff)
	);
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0 , nullptr);

	//インデックスバッファをマッピング

	result = indexBuff->Map(0 , nullptr , (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//座標をコピー
	}
	//繋がりを解除
	indexBuff->Unmap(0 , nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Model::Draw() {
	//頂点バッファビューの設定コマンド
	dx12base.GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView);

	//インデックスバッファビューの設定コマンド
	dx12base.GetCmdList()->IASetIndexBuffer(&ibView);

	//描画コマンド
	dx12base.GetCmdList()->DrawIndexedInstanced(indices.size() , 1 , 0 , 0 , 0);
}

//アクセッサ
D3D12_RESOURCE_DESC Model::GetResDesc() {
	return resDesc;
}

D3D12_VERTEX_BUFFER_VIEW Model::GetVbView() {
	return vbView;
}

D3D12_INDEX_BUFFER_VIEW Model::GetIbView() {
	return ibView;
}
