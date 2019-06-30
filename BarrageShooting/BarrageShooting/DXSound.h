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
		//���ʐݒ�
		void SetVolume(LONG volume) { mDsBuffer->SetVolume(volume); }
		//�����ŏ�����Đ��ł���悤�ɂ���
		void ResetSound();
		void Play(bool isLoop = true);
		void Stop();
	private:
		IDirectSound8* mDirectSound = nullptr;
		IDirectSoundBuffer8* mDsBuffer = nullptr;
		bool OpenWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);
	};

}

