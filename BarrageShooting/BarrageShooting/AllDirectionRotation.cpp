#include "stdafx.h"
#include "AllDirectionRotation.h"
#include "DXGameObject.h"
#include "BulletPool.h"

using namespace MyDirectX;
AllDirectionRotation::AllDirectionRotation()
{
	mName = BarrageName::AllDirectionsRotation;
	mCoolTime = 30;
	mBulletSpeed = 0.01f;
	mAllAngle = 360.0f;
	mBulletNum = 36;
	mAngle = mAllAngle / mBulletNum;
	mShotNum = 1;
	auto radian = ToRadian(mAngle);
	mCosAngle = cos(radian);
	mSinAngle = sin(radian);
	mShiftCosAngle = cos(radian / 5);
	mShiftSinAngle = sin(radian / 5);
	mCount = 0;
	mCenterVectolX = 0.0f;
	mCenterVectolY = -1.0f;
	auto len = sqrtf(mCenterVectolX * mCenterVectolX + mCenterVectolY * mCenterVectolY);
	mCenterVectolX /= len;
	mCenterVectolY /= len;
	mCenterVectolX *= mBulletSpeed;
	mCenterVectolY *= mBulletSpeed;
	mBulletData.tag = EnemyBullet;
	mBulletData.ScaleRatio(0.3f);
	mBulletData.texturePath = WHITE_TO_RED_BULLET_PATH;
	mBulletData.transform.Position.z = -0.01f;
}

void AllDirectionRotation::CreateBarrage(BulletPool * pool, DXGameObject * player, DXGameObject * enemy, float scaleRate)
{
	mShotCount++;
	mBulletData.transform = *enemy->GetTransform();
	mBulletData.transform.Scale.x /= 2.0f / scaleRate;
	mBulletData.transform.Scale.y /= 2.0f / scaleRate;
	
	//複数発射する用テストコード
	for (int i = 0; i < mBulletNum; i++)
	{
		auto vecX2 = mCenterVectolX * mCosAngle - mCenterVectolY * mSinAngle;
		auto vecY2 = mCenterVectolX * mSinAngle + mCenterVectolY * mCosAngle;
		mCenterVectolX = vecX2;
		mCenterVectolY = vecY2;
		mBulletData.xVectol = mCenterVectolX;
		mBulletData.yVectol = mCenterVectolY;
		//弾を出す
		pool->GetBullet(mBulletData);
	}
	auto shiftX = mCenterVectolX * mShiftCosAngle - mCenterVectolY * mShiftSinAngle;
	auto shiftY = mCenterVectolX * mShiftSinAngle + mCenterVectolY * mShiftCosAngle;
	mCenterVectolX = shiftX;
	mCenterVectolY = shiftY;
}
