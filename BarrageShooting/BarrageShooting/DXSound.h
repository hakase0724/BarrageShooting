#pragma once
#include <mmsystem.h>
#include <dsound.h>
#include <windows.h>
#include <tchar.h>
namespace MyDirectX
{
	class DXSound
	{
	public:
		DXSound(HWND hwnd);
		DXSound(HWND hwnd, LPWSTR sourcePath);
		~DXSound() {};
		//音量設定
		void SetVolume(LONG volume) { mDsBuffer->SetVolume(volume); }
		//音を最初から再生できるようにする
		void ResetSound();
		void Play(bool isLoop = true);
		void Stop();
	private:
		IDirectSound8* mDirectSound = nullptr;
		IDirectSoundBuffer8* mDsBuffer = nullptr;
		bool OpenWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);
	};

}


