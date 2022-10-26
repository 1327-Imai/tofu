#include "Model.h"

//�����o�֐�
void Model::LoadModel() {

	//���_�f�[�^
	vertices = {
		//�O
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{-1.0f , 1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{1.0f , -1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{1.0f , 1.0f , -1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3
		//��
		{{-1.0f , -1.0f , 1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{-1.0f , 1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{1.0f , -1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3
		//��
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{-1.0f , -1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{-1.0f , 1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{-1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3
		//�E
		{{1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{1.0f , -1.0f , 1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{1.0f , 1.0f , -1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3
		//��
		{{-1.0f , 1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{1.0f , 1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{-1.0f , 1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{1.0f , 1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3
		//��
		{{-1.0f , -1.0f , -1.0f} , {} , {0.0f , 1.0f}} ,//���� �C���f�b�N�X0
		{{1.0f , -1.0f , -1.0f} , {} , {0.0f , 0.0f}} ,//���� �C���f�b�N�X1
		{{-1.0f , -1.0f , 1.0f} , {} , {1.0f , 1.0f}} ,//�E�� �C���f�b�N�X2
		{{1.0f , -1.0f , 1.0f} , {} , {1.0f , 0.0f}} ,//�E�� �C���f�b�N�X3

	};

	//�C���f�b�N�X�f�[�^
	indices = {
		//�O
		0 , 1 , 2 , //�O�p�`1��
		1 , 3 , 2 , //�O�p�`2��
		//���
		4 , 6 , 5 ,
		5 , 6 , 7 ,
		//��
		8 , 9 , 10 ,
		9 , 11 , 10 ,
		//�E
		12 , 14 , 13 ,
		13 , 14 , 15 ,
		//��
		16 , 18 , 17 ,
		17 , 18 , 19 ,
		//��
		20 , 21 , 22 ,
		21 , 23 , 22 ,
	};

}

void Model::LoadModel(const char* fileName) {

	if (fileName) {

		//�J�������t�@�C���̊g���q���擾
		size_t fileNameLen = strlen(fileName);
		strcpy_s(fileType , fileName + fileNameLen - 4);


#pragma region//.obj�t�@�C���̏ꍇ
		if (strcmp(fileType , ".obj") == 0) {

				////�t�@�C���X�g���[��
				//std::fstream file;
				////.obj�t�@�C�����J��
				//file.open(fileName);
				////�t�@�C���I�[�v�����s���`�F�b�N
				//if (file.fail()) {
				//	assert(0);
				//}

				//std::vector<XMFLOAT3>positions;	//���_���W
				//std::vector<XMFLOAT3>normals;	//�@���x�N�g��
				//std::vector<XMFLOAT2>texcoad;	//�e�N�X�`��UV
				////1�s���ǂݍ���
				//std::string line;
				//while (getline(file , line)) {

				//	//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				//	std::istringstream line_stream(line);

				//	//���p�X�y�[�X��؂�ōs�̐擪�������擾
				//	std::string key;
				//	getline(line_stream , key , ' ');

				//	//�擪������v�Ȃ璸�_���W
				//	if (key == "v") {

				//		//X,Y,Z���W�ǂݍ���
				//		XMFLOAT3 position{};
				//		line_stream >> position.x;
				//		line_stream >> position.y;
				//		line_stream >> position.z;

				//		//���W�f�[�^�ɒǉ�
				//		positions.emplace_back(position);

				//		//���_�f�[�^�ɒǉ�
				//		Vertex vertex{};
				//		vertex.pos = position;
				//		vertices.emplace_back(vertex);

				//	}

				//	//�擪������f�Ȃ�|���S��(�O�p�`)
				//	if (key == "f") {

				//		//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
				//		std::string index_string;
				//		while (getline(line_stream , index_string , ' ')) {

				//			//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				//			std::istringstream index_stream(index_string);
				//			unsigned short indexPosition;
				//			index_stream >> indexPosition;

				//			//���_�C���f�b�N�X�ɒǉ�
				//			indices.emplace_back(indexPosition - 1);

				//		}

				//	}

				//}
				//file.close();

			//�t�@�C���X�g���[��
			std::fstream file;
			//.obj�t�@�C�����J��
			file.open(fileName);
			//�t�@�C���I�[�v�����s���`�F�b�N
			if (file.fail()) {
				assert(0);
			}

			std::vector<XMFLOAT3>positions;	//���_���W
			std::vector<XMFLOAT3>normals;	//�@���x�N�g��
			std::vector<XMFLOAT2>texcoords;	//�e�N�X�`��UV
			//1�s���ǂݍ���
			std::string line;
			while (getline(file , line)) {

				//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream line_stream(line);

				//���p�X�y�[�X��؂�ōs�̐擪�������擾
				std::string key;
				getline(line_stream , key , ' ');

				//�擪������v�Ȃ璸�_���W
				if (key == "v") {

					//X,Y,Z���W�ǂݍ���
					XMFLOAT3 position{};
					line_stream >> position.x;
					line_stream >> position.y;
					line_stream >> position.z;

					//���W�f�[�^�ɒǉ�
					positions.emplace_back(position);

				}

				//�擪������vt�Ȃ�e�N�X�`��
				if (key == "vt") {
					//U,V�����ǂݍ���
					XMFLOAT2 texcoord{};
					line_stream >> texcoord.x;
					line_stream >> texcoord.y;

					//V�������]
					texcoord.y = 1.0f - texcoord.y;

					//�e�N�X�`�����W�f�[�^�ɒǉ�
					texcoords.emplace_back(texcoord);

				}

				if (key == "vn") {
					//X,Y,Z�����ǂݍ���
					XMFLOAT3 normal{};
					line_stream >> normal.x;
					line_stream >> normal.y;
					line_stream >> normal.z;

					//�@���x�N�g���f�[�^�ɒǉ�
					normals.emplace_back(normal);

				}

				//�擪������f�Ȃ�|���S��(�O�p�`)
				if (key == "f") {

					//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
					std::string index_string;
					while (getline(line_stream , index_string , ' ')) {

						//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
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

						//�C���f�b�N�X�f�[�^�̒ǉ�
						indices.emplace_back((unsigned short)indices.size());
					}
				}
			}
			file.close();
		}
#pragma endregion
		else {
			//�Ή����Ă��Ȃ����f���̓G���[��f��
			assert(nullptr);
		}
	}
	else {
		LoadModel();
	}
}

void Model::Initialize() {

	//�@���̌v�Z
	for (int i = 0; i < indices.size() / 3; i++) {

		//�O�p�`�̃C���f�b�N�X�����o���Ĉꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1 , p0);
		XMVECTOR v2 = XMVectorSubtract(p2 , p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1 , v2);

		//���K��
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal , normal);
		XMStoreFloat3(&vertices[index1].normal , normal);
		XMStoreFloat3(&vertices[index2].normal , normal);

	}


	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&heapProp ,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE ,
		&resDesc ,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ ,
		nullptr ,
		IID_PPV_ARGS(&indexBuff)
	);
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0 , nullptr , (void**)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];	//���W���R�s�[
	}
	//�q���������
	vertBuff->Unmap(0 , nullptr);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O

	result = indexBuff->Map(0 , nullptr , (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];	//���W���R�s�[
	}
	//�q���������
	indexBuff->Unmap(0 , nullptr);

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_�P���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Model::Draw() {
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dx12base.GetCmdList()->IASetVertexBuffers(0 , 1 , &vbView);

	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	dx12base.GetCmdList()->IASetIndexBuffer(&ibView);

	//�`��R�}���h
	dx12base.GetCmdList()->DrawIndexedInstanced(indices.size() , 1 , 0 , 0 , 0);
}

//�A�N�Z�b�T
D3D12_RESOURCE_DESC Model::GetResDesc() {
	return resDesc;
}

D3D12_VERTEX_BUFFER_VIEW Model::GetVbView() {
	return vbView;
}

D3D12_INDEX_BUFFER_VIEW Model::GetIbView() {
	return ibView;
}
