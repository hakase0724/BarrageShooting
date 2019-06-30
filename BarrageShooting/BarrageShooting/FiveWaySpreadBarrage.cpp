#include "stdafx.h"
#include "FiveWaySpreadBarrage.h"
#include "DXGameObject.h"
#include "BulletPool.h"

using namespace MyDirectX;
FiveWaySpreadBarrage::FiveWaySpreadBarrage()
{
	mName = BarrageName::FiveWaySpread;
	mCoolTime = 10;
	mBulletSpeed = 0.01f;
	mBulletNum = 5;
	mShotNum = 3;
	mAllAngle = 90.0f;
	mAngle = mAllAngle / mBulletNum;
	auto radian = ToRadian(mAngle);
	mCosAngle = cos(radian);
	mSinAngle = sin(radian);
	mShiftCosAngle = cos(-mAllAngle / 2);
	mShiftSinAngle = sin(-mAllAngle / 2);
	mCount = 0;

	mBulletData.tag = EnemyBullet;
	mBulletData.ScaleRatio(0.3f);
	mBulletData.texturePath = BLUE_TO_LIGHTBLUE_BULLET_PATH;
	mBulletData.transform.Position.z = -0.01f;
}

void FiveWaySpreadBarrage::CreateBarrage(BulletPool * pool, DXGameObject * player, DXGameObject * enemy, float scaleRate)
{
	mShotCount++;
	//自分と相手の座標から目標へ向かうベクトル作成
	auto target = player->GetTransform()->Position;
	auto myPos = enemy->GetTransform()->Position;
	//敵から自機へのベクトル
	auto vecX = (target.x - myPos.x);
	auto vecY = (target.y - myPos.y);
	//適当に左右に揺らす
	vecX += vecX * sinf((float)mShotCount);
	//ベクトルの正規化
	Normalization(vecX, vecY);
	//弾の速度を設定
	vecX *= mBulletSpeed;
	vecY *= mBulletSpeed;
	//前方に打ちたいから最初のベクトルをずらす
	auto shiftX = vecX * mShiftCosAngle - vecY * mShiftSinAngle;
	auto shiftY = vecX * mShiftSinAngle + vecY * mShiftCosAngle;
	vecX = shiftX;
	vecY = shiftY;
	//弾データ
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
