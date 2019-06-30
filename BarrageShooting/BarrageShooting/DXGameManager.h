#pragma once
#include "DXResourceManager.h"
#include "DXExcuter.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "BulletPool.h"

namespace MyDirectX
{
	class DXGameManager
	{
	public:
		DXGameManager(HWND hwnd);
		~DXGameManager() 
		{
			for (auto &scene : mSceneList) scene.reset();
			mDXExcuter.reset();
			mDXResourceManager.reset();
		};
		void Initialize();
		//�Q�[�����[�v�����s
		//�I������false��Ԃ�
		bool Update();
		void UpdateFPS() { mDXResourceManager->UpdateFPS(); }
	private:
		//�V�[���؂�ւ�
		void SceneChange();
		//���V�[����
		int mSceneCount = 0;
		//���ݓo�^���Ă���V�[���̔ԍ�
		int mSceneIndex = 0;
		//�V�[���̃��X�g
		std::vector<std::unique_ptr<Scene>> mSceneList;
		//�@�\�N���X���܂Ƃ߂��N���X
		std::unique_ptr<DXResourceManager> mDXResourceManager;
		//�Q�[�����[�v�����s����N���X
		std::unique_ptr<DXExcuter> mDXExcuter;
	};

}

