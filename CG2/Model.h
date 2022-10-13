#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>


class Model
{
public:

	std::vector<uint16_t> indices;

	// プログラムで作成したモデルを生成する用の関数
	void CreateDefaultModel();

	// モデルを読み込んで生成する関数
	void CreateModel(const std::string t);

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

};

