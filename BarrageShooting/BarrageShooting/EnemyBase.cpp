#include "stdafx.h"
#include "EnemyBase.h"
#include "PlayScene.h"

using namespace MyDirectX;

void EnemyBase::Initialize(DXGameObject * gameObject)
{
	mGameObject = gameObject;
	mId = mGameObject->GetID();
	mWaitCount = mCoolCount;
	mPlayScene = dynamic_cast<PlayScene*>(mGameObject->GetScene());
}

bool EnemyBase::IsBarrageEnd()
{
	mShotCount++;
	if(mShotCount >= mBarrage->GetShotNum())
	{
		mShotCount = 0;
		return true;
	}
	else return false;
}

void EnemyBase::OnCollisionEnter2D(Collider2D* col)
{
	//����������1�_���[�W�󂯂�
	Damage(1.0);
}

void EnemyBase::OnDisable()
{
	mPlayScene->AddScore(mScore);
	auto transform = *mGameObject->GetTransform();
	mPlayScene->CreateExplosionEffect(transform.Position);
	switch (mDropItemType)
	{
	case PowerUp:
		mPlayScene->CreatePowerUp(transform.Position);
		break;
	case BombItem:
		mPlayScene->CreateBomb(transform.Position);
		break;
	case Score:
		mPlayScene->CreateScoreItem(transform.Position);
		break;
	default:
		break;
	}
}

void EnemyBase::Damage(double damage)
{
	mHitPoint -= damage;
	//�̗͂��Ȃ��Ȃ����玩�g�̃A�N�e�B�u��؂�
	if (mHitPoint <= 0)
	{
		mGameObject->SetEnable(false);
	}
}