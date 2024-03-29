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

	//長押しされているか
	if(mPlayerCom->IsLongPush())
	{
		//されていたら前方に
		mTransform->Position.y += mYOffset;
		return;
	}
	else
	{
		//されていなければ横に
		mTransform->Position.x += mXOffset;
	}

	//プレイヤーが打っていたら一緒に打つ
	if(mPlayerCom->IsShot())
	{
		BULLET_SETTING_DATA data;
		data.transform = *mGameObject->GetTransform();
		data.tag = PlayerBullet;
		data.xVectol = mXVectol;
		data.yVectol = 0.05f;
		data.texturePath = BLUE_TO_LIGHTBLUE_BULLET_PATH;
		data.ScaleRatio(0.3f);
		//弾を出す
		mBulletPool->GetBullet(data);
	}
}

void OptionUnit::SetPlayer(DXGameObject * player)
{
	mPlayerGameObject = player;
	mPlayerCom = player->GetComponent<Player>();
}
