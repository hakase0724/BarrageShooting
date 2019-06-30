#include "stdafx.h"
#include "DXSound.h"

#pragma comment ( lib, "winmm.lib" )
#pragma comment (lib ,"dsound.lib")
#pragma comment (lib,"dxguid.lib")

using namespace MyDirectX;
DXSound::DXSound(HWND hwnd)
{
	// Wave�t�@�C���I�[�v��
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	auto path = (LPWSTR)BGM_PATH;
	auto open = OpenWave(path, wFmt, &pWaveData, waveSize);

	HRESULT hr = DirectSoundCreate8(NULL,&mDirectSound,NULL);
	
	mDirectSound->SetCooperativeLevel(hwnd,DSSCL_NORMAL);

	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = 0;
	dsBufferDesc.dwBufferBytes = waveSize;
	dsBufferDesc.dwReserved = 0;
	dsBufferDesc.lpwfxFormat = &wFmt;
	dsBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* tmpBuffer = 0;
	hr = mDirectSound->CreateSoundBuffer(&dsBufferDesc,&tmpBuffer,NULL);
	tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8,(void**)&mDsBuffer);
	tmpBuffer->Release();

	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == mDsBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) 
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		mDsBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	// �����͂�������Ȃ�
	delete[] pWaveData;
}

DXSound::DXSound(HWND hwnd, LPWSTR sourcePath)
{
	// Wave�t�@�C���I�[�v��
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	auto open = OpenWave(sourcePath, wFmt, &pWaveData, waveSize);

	HRESULT hr = DirectSoundCreate8(NULL, &mDirectSound, NULL);

	mDirectSound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);

	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	dsBufferDesc.dwBufferBytes = waveSize;
	dsBufferDesc.dwReserved = 0;
	dsBufferDesc.lpwfxFormat = &wFmt;
	dsBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* tmpBuffer = 0;
	hr = mDirectSound->CreateSoundBuffer(&dsBufferDesc, &tmpBuffer, NULL);
	tmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&mDsBuffer);
	tmpBuffer->Release();

	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == mDsBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		mDsBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	// �����͂�������Ȃ�
	delete[] pWaveData;
}

void DXSound::ResetSound()
{
	mDsBuffer->SetCurrentPosition(0);
}

void DXSound::Play(bool isLoop)
{
	if(isLoop)
	{
		//���[�v�Đ�
		mDsBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		mDsBuffer->Play(0, 0, 0);
	}
}

void DXSound::Stop()
{
	mDsBuffer->Stop();
	//�Đ��ʒu��擪�ɖ߂�
	mDsBuffer->SetCurrentPosition(0);
}

bool DXSound::OpenWave(TCHAR * filepath, WAVEFORMATEX & waveFormatEx, char ** ppData, DWORD & dataSize)
{
	if (filepath == 0) 
	{
		auto text = _T("Wave�t�@�C���p�X���Ȃ�") + *filepath;
		MessageBox(NULL, text, _T("Init"), MB_OK);
		return false;
	} 

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;
	// Wave�t�@�C���I�[�v��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio) 
	{
		// �t�@�C���I�[�v�����s
		MessageBox(NULL, _T("Wave�t�@�C���I�[�v�����s"), _T("Init"), MB_OK);
		return false; 
	}
		

// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) 
	{
		MessageBox(NULL, _T("RIFF�`�����N�������s"), _T("Init"), MB_OK);
		mmioClose(hMmio, 0);
		return false;
	}

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) 
	{
		MessageBox(NULL, _T("�t�H�[�}�b�g�`�����N�������s"), _T("Init"), MB_OK);
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
	if (size != fmsize) 
	{
		MessageBox(NULL, _T("�T�C�Y�s��v"), _T("Init"), MB_OK);
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) 
	{
		MessageBox(NULL, _T("�f�[�^�`�����N�������s"), _T("Init"), MB_OK);
		mmioClose(hMmio, 0);
		return false;
	}
	*ppData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
	if (size != dataChunk.cksize) 
	{
		MessageBox(NULL, _T("�T�C�Y�s��v"), _T("Init"), MB_OK);
		delete[] * ppData;
		return false;
	}
	dataSize = size;

	// �n���h���N���[�Y
	mmioClose(hMmio, 0);
	return true;
}