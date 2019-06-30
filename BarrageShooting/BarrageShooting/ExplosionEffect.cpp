#include "stdafx.h"
#include "ExplosionEffect.h"
#include "DXGameObject.h"
#include "DXTexture.h"
#include "DXAnimation.h"
#include "ExplosionEffectPool.h"

using namespace MyDirectX;

void ExplosionEffect::Initialize(DXGameObject * gameObject)
{
	mGameObject = gameObject;
	auto exTex = mGameObject->AddComponent<DXTexture>();
	auto exAnim = mGameObject->AddComponent<DXAnimation>();
	exAnim->SetAnimationTexture(exTex);
	exAnim->SetAnimationFile(EXPLOSION1_PATH);
	exAnim->SetAnimationFile(EXPLOSION2_PATH);
	exAnim->SetAnimationFile(EXPLOSION3_PATH);
	exAnim->SetAnimationFile(EXPLOSION4_PATH);
	exAnim->SetLoop(false);
	exAnim->SetTextureChangeCount(2);
}

void ExplosionEffect::OnDisable()
{
	mPool->ReturnExplosionEffect(mGameObject);
}
