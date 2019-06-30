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
		//ゲームループを実行
		//終了時にfalseを返す
		bool Update();
		void UpdateFPS() { mDXResourceManager->UpdateFPS(); }
	private:
		//シーン切り替え
		void SceneChange();
		//総シーン数
		int mSceneCount = 0;
		//現在登録しているシーンの番号
		int mSceneIndex = 0;
		//シーンのリスト
		std::vector<std::unique_ptr<Scene>> mSceneList;
		//機能クラスをまとめたクラス
		std::unique_ptr<DXResourceManager> mDXResourceManager;
		//ゲームループを実行するクラス
		std::unique_ptr<DXExcuter> mDXExcuter;
	};

}

