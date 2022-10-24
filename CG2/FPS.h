#pragma once
#include "ErrorException.h"

#pragma comment(lib, "winmm.lib")

class FPS {
  private:
	// CPU周波数
	LARGE_INTEGER cpuClock{};
	//計測開始時間
	LARGE_INTEGER timeStart{};
	//計測終了時間
	LARGE_INTEGER timeEnd{};
	//固定する時間
	float frameTime = 1 / 60.0f;
	// FPS値
	float fps;

  public:
	/// <summary>
	/// FPS制御初期化
	/// </summary>
	void FpsControlBegin();

	/// <summary>
	/// FPS制御
	/// </summary>
	void FpsControlEnd();

	/// <summary>
	/// フレームレートを設定
	/// </summary>
	/// <param name="fps_">フレームレート</param>
	void SetFrameRate(float fps_);

	/// <summary>
	/// フレームレートを取得
	/// </summary>
	/// <returns>フレームレート</returns>
	float GetFrameRate();
};