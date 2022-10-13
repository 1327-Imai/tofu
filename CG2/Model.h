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

	// �v���O�����ō쐬�������f���𐶐�����p�̊֐�
	void CreateDefaultModel();

	// ���f����ǂݍ���Ő�������֐�
	void CreateModel(const std::string t);

	// �}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

};

