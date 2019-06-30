#include "stdafx.h"
#include "AllDirectionSync.h"
#include "DXGameObject.h"
#include "BulletPool.h"

using namespace MyDirectX;

AllDirectionSync::AllDirectionSync()
{
	mName = BarrageName::AllDirectionsSync;
	mCoolTime = 30;
	mBulletSpeed = 0.01f;
	mAllAngle = 360.0f;
	mBulletNum = 36;
	mAngle = mAllAngle / mBulletNum;
	mShotNum = 1;
	auto radian = ToRadian(mAngle);
	mCosAngle = cos(radian);
	mSinAngle = sin(radian);
	mCount = 0;
}

void AllDirectionSync::CreateBarrage(BulletPool * pool, DXGameObject * player, DXGameObject * enemy, float scaleRate)
{
	mShotCount++;
	//自分と相手の座標から目標へ向かうベクトル作成
	auto target = player->GetTransform()->Position;
	auto myPos = enemy->GetTransform()->Position;
	//敵から自機へのベクトル
	auto vecX = (target.x - myPos.x);
	auto vecY = (target.y - myPos.y);
	//ベクトルの正規化
	auto len = sqrtf(vecX * vecX + vecY * vecY);
	vecX /= len;
	vecY /= len;
	//弾の速度を設定
	vecX *= mBulletSpeed;
	vecY *= mBulletSpeed;
	BULLET_SETTING_DATA data;
	data.transform = *enemy->GetTransform();
	data.transform.Position.z = -0.01f;
	data.transform.Scale.x /= 2.0f / scaleRate;
	data.transform.Scale.y /= 2.0f / scaleRate;
	data.tag = EnemyBullet;
	data.texturePath = WHITE_TO_RED_BULLET_PATH;
	data.ScaleRatio(0.3f);
	data.color = mColor;
	//複数発射する用テストコード
	for (int i = 0; i < mBulletNum; i++)
	{
		auto vecX2 = vecX * mCosAngle - vecY * mSinAngle;
		auto vecY2 = vecX * mSinAngle + vecY * mCosAngle;
		vecX = vecX2;
		vecY = vecY2;
		data.xVectol = vecX;
		data.yVectol = vecY;
		//弾を出す
		pool->GetBullet(data);
	}
}
