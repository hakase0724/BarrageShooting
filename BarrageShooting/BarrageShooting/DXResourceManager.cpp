#include "stdafx.h"
#include "DXResourceManager.h"
#include "DXGameObject.h"

using namespace MyDirectX;

DXResourceManager::DXResourceManager(HWND hwnd)
{
	//DirectX���\�[�X�Ǘ�
	mDXManager = std::make_unique<DXManager>(hwnd);
	//FPS�J�E���^�[�𐶐�
	mFPSCountor = std::make_unique<FPSCountor>();
	//CSV�ǂݍ��݃N���X
	mCSVLoader = std::make_unique<CSVLoader>();
	//BGM�Ǘ��N���X
	mBGMDXSound = std::make_unique<DXSound>(hwnd, (LPWSTR)BGM_PATH);
	//�V���b�g���Ǘ��N���X
	mSEDXSound = std::make_unique<DXSound>(hwnd, (LPWSTR)SHOT_PATH);
	//���j��
	mExplosionEffectSound = std::make_unique<DXSound>(hwnd, (LPWSTR)EXPLOSION_PATH);
	//���ʂ�������
	mSEDXSound->SetVolume(-1000);
	
}

DXGameObject * DXResourceManager::Instantiate()
{
	DXGameObject* game = new DXGameObject(this);
	//�e�I�u�W�F�N�g��ID���~��
	game->SetID(mGameObjectCount);
	mGameObjectCount++;
	return game;
}

