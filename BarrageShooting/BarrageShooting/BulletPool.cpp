#include "stdafx.h"
#include "BulletPool.h"
#include "MeshRenderer.h"
#include "DXTexture.h"
#include <algorithm>
#include <ppl.h>

using namespace MyDirectX;

void BulletPool::CreatePreBullets(int preNum)
{
	for (int i = 0; i < preNum; i++)
	{
		CreateBullet();
	}
}

DXGameObject * BulletPool::GetBullet(BULLET_SETTING_DATA data)
{
	DXGameObject* pGame;
	//�I�u�W�F�N�g������Ύg���܂킷
	//�Ȃ���ΐV�������
	if (mBulletList.size() <= 0)
	{
		pGame = CreateBullet();
	}
	else
	{
		pGame = mBulletList.back();
		mBulletList.pop_back();
	}
	auto pGameTransform = pGame->GetTransform();
	auto scale = data.transform.Scale;
	//�傫���ݒ�
	scale.x *= data.scaleXRatio;
	scale.y *= data.scaleYRatio;
	scale.z *= data.scaleZRatio;
	//�K�p
	pGame->SetTag(data.tag);
	pGameTransform->Position = data.transform.Position;
	pGameTransform->Scale = scale;
	pGameTransform->Rotation = data.transform.Rotation;
	pGame->GetComponent<MeshRenderer>()->SetDefaultColor(data.color);
	pGame->InitializeComponent();
	auto pBullet = pGame->GetComponent<Bullet>();
	//�i�s�x�N�g���ݒ�
	pBullet->SetVectol(data.xVectol, data.yVectol);
	//�\������摜�ݒ�
	pBullet->SetTexture(data.texturePath.c_str());
	//x���W�Œ�
	if (data.isXFixed)
	{
		pBullet->FixedXAxiz(&data.pTransform->Position.x);
	}
	//y���W�Œ�
	if (data.isYFixed)
	{
		pBullet->FixedYAxiz(&data.pTransform->Position.y);
	}	
	pGame->SetEnable(true);
	return pGame;
}

void BulletPool::ReturnBullet(DXGameObject * bullet)
{
	mBulletList.push_back(bullet);
}

DXGameObject * BulletPool::CreateBullet()
{
	auto game = mScene->Instantiate();
	game->SetName("Bullet");
	game->SetTag(Tag::PlayerBullet);
	auto col = game->AddComponent<SquareCollider2D>();
	col->SetOneSide(col->GetOneSide() / 10.0f);
	auto bullet = game->AddComponent<Bullet>();
	bullet->SetBulletPool(this);
	auto tex = game->AddComponent<DXTexture>();
	bullet->SetTextureComponent(tex);
	bullet->SetTexture(BULLET_PATH);
	mBulletList.push_back(game);
	return game;
}