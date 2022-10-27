#pragma once
#include "DX12base.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <map>
#include <combaseapi.h>
#include <Windows.h>
#include <cassert>
#include <iterator>

//�`�����N�w�b�_
struct ChunkHeader {
	char id[4];		//�`�����N����ID
	int32_t size;	//�`�����N�T�C�Y
};

//RIFF�w�b�_�`�����N
struct RiffHeader {
	ChunkHeader chunk;	//"RIFF"
	char type[4];		//"WAVE"
};

//FMT�`�����N
struct FormatChunk {
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
};

//�����f�[�^
struct SoundData {
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	//�o�b�t�@�T�C�Y
	unsigned int bufferSize;
};

class SoundManager
{
public:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	static const int kMaxSoundData = 256;
	void Initialize();
	//�����ǂݍ���
	SoundData SoundLoadWave(const char* filename);
	//�����Đ�
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData, bool loop = false, float volume = 1.0f);
	// ������~
	void StopWave(const SoundData& soundData);
	//void StopWave(const std::string& filename);
	//�������
	void SoundUnload(SoundData& soundData);
	//xAudio2�̉��
	void End();

private:
	std::map<std::string, SoundData> soundDatas_;
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	//�g�`�t�H�[�}�b�g����SourceVoice�̐���
	IXAudio2SourceVoice* sourceVoice = nullptr;
	HRESULT result;
};