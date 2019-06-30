#include "stdafx.h"
#include "DXGameManager.h"

using namespace MyDirectX;
DXGameManager::DXGameManager(HWND hwnd)
{
	//�@�\�N���X���܂Ƃ߂�����
	mDXResourceManager = std::make_unique<DXResourceManager>(hwnd);
	//�Q�[�����[�v�����s����N���X
	mDXExcuter = std::make_unique<DXExcuter>();
	mDXExcuter->SetDXResourceManager(mDXResourceManager.get());
	
	//�^�C�g���V�[������
	auto title = new TitleScene();
	title->SetDXResourceManager(mDXResourceManager.get());
	title->Init();
	mSceneList.push_back(std::unique_ptr<Scene>(title));
	//�v���C�V�[������
	auto play = new PlayScene();
	play->SetDXResourceManager(mDXResourceManager.get());
	play->Init();
	mSceneList.push_back(std::unique_ptr<Scene>(play));
	//���U���g�V�[������
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
	//���݂̓��͂��擾
	mDXResourceManager->SetInputState();
	//���������s
	mDXExcuter->Excute();
	//�G�X�P�[�v��������Ă�����I������
	if (mDXResourceManager->GetKey(DIK_ESCAPE)) 
	{
		mDXExcuter.reset();
		mDXResourceManager->GetDXManager()->ResourceRelese();
		return false;
	}
	//�V�[�����I�����Ă�����؂�ւ���
	if (mDXExcuter->IsSceneEnd()) SceneChange();
	//���t���[���̂��߂Ɍ��݂̓��͏󋵂�ۑ�
	mDXResourceManager->SetPreBuffer();
	return true;
}

void DXGameManager::SceneChange()
{
	mSceneIndex = (++mSceneIndex) % mSceneCount;
	mDXExcuter->SetScene(mSceneList[mSceneIndex].get());
}
