#include "stdafx.h"
#include "PowerUpItem.h"
#include "DXGameObject.h"
#include "Collider2D.h"
#include "Player.h"
#include "PowerUpItemPool.h"

using namespace MyDirectX;

void PowerUpItem::Initialize(DXGameObject * gameObject)
{
	mGameObject = gameObject;
	mId = mGameObject->GetID();
}

void PowerUpItem::OnCollisionEnter2D(Collider2D * col)
{
	auto game = col->GetGameObject();
	auto player = game->GetComponent<Player>();
	if (player == nullptr) return;
	player->PowerUp();
	mGameObject->SetEnable(false);
}

void PowerUpItem::OnDisable()
{
	mPool->ReturnPowerUpItem(mGameObject);
}
