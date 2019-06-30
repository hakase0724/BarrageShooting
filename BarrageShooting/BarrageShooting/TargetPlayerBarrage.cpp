#include "stdafx.h"
#include "TargetPlayerBarrage.h"
#include "DXGameObject.h"
#include "BulletPool.h"

using namespace MyDirectX;
TargetPlayerBarrage::TargetPlayerBarrage()
{
	mName = BarrageName::TargetPlayer;
	mCoolTime = 30;
	mBulletSpeed = 0.02f;
	mShotNum = 5;
	mCount = 0;
}

void TargetPlayerBarrage::CreateBarrage(BulletPool * pool, DXGameObject * player, DXGameObject * enemy, float scaleRate)
{
	mShotCount++;
	//�����Ƒ���̍��W����ڕW�֌������x�N�g���쐬
	auto target = player->GetTransform()->Position;
	auto myPos = enemy->GetTransform()->Position;
	//�G���玩�@�ւ̃x�N�g��
	auto vecX = (target.x - myPos.x);
	auto vecY = (target.y - myPos.y);
	//�x�N�g���̐��K��
	auto len = sqrtf(vecX * vecX + vecY * vecY);
	vecX /= len;
	vecY /= len;
	//�e�̑��x��ݒ�
	vecX *= mBulletSpeed;
	vecY *= mBulletSpeed;
	//�e�f�[�^
	BULLET_SETTING_DATA data;
	data.transform = *enemy->GetTransform();
	data.transform.Position.z = -0.01f;
	data.transform.Scale.x /= 1.0f / scaleRate;
	data.transform.Scale.y /= 1.0f / scaleRate;
	data.tag = EnemyBullet;
	data.xVectol = vecX;
	data.yVectol = vecY;
	data.texturePath = mBulletTexturePath;
	data.ScaleRatio(0.3f);
	data.color = mColor;
	//�f�[�^��n���e���󂯎��
	pool->GetBullet(data);
}