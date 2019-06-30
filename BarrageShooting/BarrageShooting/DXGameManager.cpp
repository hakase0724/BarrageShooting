#include "stdafx.h"
#include "DXGameManager.h"

using namespace MyDirectX;
DXGameManager::DXGameManager(HWND hwnd)
{
	//機能クラスをまとめたもの
	mDXResourceManager = std::make_unique<DXResourceManager>(hwnd);
	//ゲームループを実行するクラス
	mDXExcuter = std::make_unique<DXExcuter>();
	mDXExcuter->SetDXResourceManager(mDXResourceManager.get());
	
	//タイトルシーン生成
	auto title = new TitleScene();
	title->SetDXResourceManager(mDXResourceManager.get());
	title->Init();
	mSceneList.push_back(std::unique_ptr<Scene>(title));
	//プレイシーン生成
	auto play = new PlayScene();
	play->SetDXResourceManager(mDXResourceManager.get());
	play->Init();
	mSceneList.push_back(std::unique_ptr<Scene>(play));
	//リザルトシーン生成
	auto result = new ResultScene();
	result->SetDXResourceManager(mDXResourceManager.get());
	result->Init();
	mSceneList.push_back(std::unique_ptr<Scene>(result));
}

void DXGameManager::Initialize()
{
	mSceneIndex = 0;
	mSceneCount = mSceneList.size();
	mDXExcuter->SetScene(mSceneList[mSceneIndex].get());
}

bool DXGameManager::Update()
{
	//現在の入力を取得
	mDXResourceManager->SetInputState();
	//処理を実行
	mDXExcuter->Excute();
	//エスケープが押されていたら終了する
	if (mDXResourceManager->GetKey(DIK_ESCAPE)) 
	{
		mDXExcuter.reset();
		mDXResourceManager->GetDXManager()->ResourceRelese();
		return false;
	}
	//シーンが終了していたら切り替える
	if (mDXExcuter->IsSceneEnd()) SceneChange();
	//次フレームのために現在の入力状況を保存
	mDXResourceManager->SetPreBuffer();
	return true;
}

void DXGameManager::SceneChange()
{
	mSceneIndex = (++mSceneIndex) % mSceneCount;
	mDXExcuter->SetScene(mSceneList[mSceneIndex].get());
}
