#include "stdafx.h"
#include "Mover.h"
#include <sstream>
using namespace MyDirectX;

void Mover::Initialize(DXGameObject * gameObject)
{
	mDXInput = gameObject->GetDXInput();
	mGameObject = gameObject;
}

void Mover::Update()
{
	auto transform = mGameObject->GetTransform();
	//移動前の座標保持
	auto prePos = transform->Position;
	//入力を受けて移動
	if (mDXInput->GetKey(DIK_LEFTARROW)) transform->Position.x -= mMoveSpeed;
	if (mDXInput->GetKey(DIK_RIGHTARROW)) transform->Position.x += mMoveSpeed;
	if (mDXInput->GetKey(DIK_UPARROW)) transform->Position.y += mMoveSpeed;
	if (mDXInput->GetKey(DIK_DOWNARROW)) transform->Position.y -= mMoveSpeed;
	//スクリーン座標取得
	auto screenPos = mGameObject->GetDXManager()->GetDXCamera()->WorldToScreenPoint(transform);
	auto screenPosX = screenPos.m128_f32[0];
	auto screenPosY = screenPos.m128_f32[1];

	if(!IsInGameScreen(screenPosX,screenPosY))
	{
		transform->Position = prePos;
	}
}

bool Mover::IsInGameScreen(float screenX, float screenY)
{
	if (cMinScreenXPos <= screenX && cMaxScreenXPos >= screenX) 
	{
		if (cMinScreenYPos <= screenY && cMaxScreenYPos >= screenY) return true;
	} 
	return false;
}
