#include "stdafx.h"
#include "SixWayRazerBarrage.h"
#include "DXGameObject.h"
#include "BulletPool.h"

using namespace MyDirectX;
SixWayRazerBarrage::SixWayRazerBarrage()
{
	mName = BarrageName::SixWayRazer;
	mCoolTime = 20;
	mBulletSpeed = 0.01f;
	mBulletNum = 6;
	mShotNum = 10;
	mAllAngle = 360.0f;
	mAngle = mAllAngle / mBulletNum;
	auto radian = ToRadian(mAngle);
	mCosAngle = cos(radian);
	mSinAngle = sin(radian);
	mCount = 0;

	mBulletData.tag = EnemyBullet;
	mBulletData.ScaleRatio(0.3f);
	mBulletData.texturePath = WHITE_TO_PURPLE_BULLET_PATH;
	mBulletData.transform.Position.z = -0.01f;
}

void SixWayRazerBarrage::CreateBarrage(BulletPool * pool, DXGameObject * player, DXGameObject * enemy, float scaleRate)
{
	mShotCount++;
	//敵から自機へのベクトル
	auto vecX = 0.0f;
	auto vecY = -1.0f;
	//ベクトルの正規化
	Normalization(vecX, vecY);
	//弾の速度を設定
	vecX *= mBulletSpeed;
	vecY *= mBulletSpeed;
	mBulletData.transform = *enemy->GetTransform();
	mBulletData.transform.Scale.x /= 2.0f / scaleRate;
	mBulletData.transform.Scale.y /= 2.0f / scaleRate;

	for (int i = 0; i < mBulletNum; i++)
	{
		auto vecX2 = vecX * mCosAngle - vecY * mSinAngle;
		auto vecY2 = vecX * mSinAngle + vecY * mCosAngle;
		vecX = vecX2;
		vecY = vecY2;
		mBulletData.xVectol = vecX;
		mBulletData.yVectol = vecY;
		//弾を出す
		pool->GetBullet(mBulletData);
	}
}
