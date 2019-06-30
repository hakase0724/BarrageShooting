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
	//オブジェクトがあれば使いまわす
	//なければ新しく作る
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
	//大きさ設定
	scale.x *= data.scaleXRatio;
	scale.y *= data.scaleYRatio;
	scale.z *= data.scaleZRatio;
	//適用
	pGame->SetTag(data.tag);
	pGameTransform->Position = data.transform.Position;
	pGameTransform->Scale = scale;
	pGameTransform->Rotation = data.transform.Rotation;
	pGame->GetComponent<MeshRenderer>()->SetDefaultColor(data.color);
	pGame->InitializeComponent();
	auto pBullet = pGame->GetComponent<Bullet>();
	//進行ベクトル設定
	pBullet->SetVectol(data.xVectol, data.yVectol);
	//表示する画像設定
	pBullet->SetTexture(data.texturePath.c_str());
	//x座標固定
	if (data.isXFixed)
	{
		pBullet->FixedXAxiz(&data.pTransform->Position.x);
	}
	//y座標固定
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
