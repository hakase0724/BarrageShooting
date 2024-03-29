#pragma once
#include<memory>
#include "DXManager.h"
#include "DXSound.h"
#include "FPSCountor.h"
#include "CSVLoader.h"

namespace MyDirectX
{
	class DXGameObject;

	class DXResourceManager
	{
	public:
		DXResourceManager(HWND hwnd);
		~DXResourceManager() 
		{
			mFPSCountor.reset();
			mBGMDXSound.reset();
			mSEDXSound.reset();
			mExplosionEffectSound.reset();
			mCSVLoader.reset();
			mDXManager.reset();
		};
		//オブジェクトを生成する
		DXGameObject* Instantiate();
		//DirectXリソース管理クラスを返す
		DXManager* GetDXManager() const { return mDXManager.get(); }
		//指定したキーが入力されたタイミングか
		bool GetKeyDown(int key) { return mDXManager->GetDXInput()->GetKeyDown(key); }
		//指定したキーが入力されているか
		bool GetKey(int key) { return mDXManager->GetDXInput()->GetKey(key); }
		//音管理クラスを返す
		DXSound* GetBGMDXSound() const { return mBGMDXSound.get(); }
		DXSound* GetSEDXSound() const { return mSEDXSound.get(); }
		DXSound* GetExplosionEffectSound() const { return mExplosionEffectSound.get(); }
		//FPSを返す
		double GetFPS() const { return mFPS; }
		//現在の入力状況を取得
		void SetInputState() { mDXManager->GetDXInput()->SetInputState(); }
		//次のフレームへ行く前に現在の入力状況を記録する
		void SetPreBuffer() { mDXManager->GetDXInput()->SetPreBuffer(); }
		//FPSを計算する
		void UpdateFPS() { mFPS = mFPSCountor->GetFPS(); }
		std::vector<LOAD_FROM_CSV_DATA> LoadData(const char* filePath) { return mCSVLoader->LoadData(filePath); }
		void SetScore(float score) { mScore = score; }
		float GetScore() const { return mScore; }
	private:
		//FPS計算結果を保持
		double mFPS = 0.0;
		float mScore = 0.0f;
		//生成したオブジェクト数
		UINT mGameObjectCount = 0;
		//各種機能クラス
		std::unique_ptr<FPSCountor> mFPSCountor;
		std::unique_ptr<DXManager> mDXManager;
		std::unique_ptr<DXSound> mBGMDXSound;
		std::unique_ptr<DXSound> mSEDXSound;
		std::unique_ptr<DXSound> mExplosionEffectSound;
		std::unique_ptr<CSVLoader> mCSVLoader;
	};
}


