#include "stdafx.h"
#include "OptionUnit.h"
#include "DXGameObject.h"


using namespace MyDirectX;

void OptionUnit::Initialize(DXGameObject * gameObject)
{
	mGameObject = gameObject;
	mTransform = mGameObject->GetTransform();
}

void OptionUnit::Initialize()
{
	if (mPlayerTransform == nullptr) 
	{
		mPlayerTransform = mPlayerGameObject->GetTransform(); 
	}
}

void OptionUnit::Update()
{
	mTransform->Position = mPlayerTransform->Position;

	//����������Ă��邩
	if(mPlayerCom->IsLongPush())
	{
		//����Ă�����O����
		mTransform->Position.y += mYOffset;
		return;
	}
	else
	{
		//����Ă��Ȃ���Ή���
		mTransform->Position.x += mXOffset;
	}

	//�v���C���[���ł��Ă�����ꏏ�ɑł�
	if(mPlayerCom->IsShot())
	{
		BULLET_SETTING_DATA data;
		data.transform = *mGameObject->GetTransform();
		data.tag = PlayerBullet;
		data.xVectol = mXVectol;
		data.yVectol = 0.05f;
		data.texturePath = BLUE_TO_LIGHTBLUE_BULLET_PATH;
		data.ScaleRatio(0.3f);
		//�e���o��
		mBulletPool->GetBullet(data);
	}
}

void OptionUnit::SetPlayer(DXGameObject * player)
{
	mPlayerGameObject = player;
	mPlayerCom = player->GetComponent<Player>();
}