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

//チャンクヘッダ
struct ChunkHeader {
	char id[4];		//チャンク毎のID
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	//"RIFF"
	char type[4];		//"WAVE"
};

//FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//波形フォーマット
};

//音声データ
struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファサイズ
	unsigned int bufferSize;
};

class SoundManager
{
public:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	static const int kMaxSoundData = 256;
	void Initialize();
	//音声読み込み
	SoundData SoundLoadWave(const char* filename);
	//音声再生
	void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData, bool loop = false, float volume = 1.0f);
	// 音声停止
	void StopWave(const SoundData& soundData);
	//void StopWave(const std::string& filename);
	//音声解放
	void SoundUnload(SoundData& soundData);
	//xAudio2の解放
	void End();

private:
	std::map<std::string, SoundData> soundDatas_;
	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	//波形フォーマットからSourceVoiceの生成
	IXAudio2SourceVoice* sourceVoice = nullptr;
	HRESULT result;
};