#include "stdafx.h"
#include "TextureRenderer.h"

using namespace DirectX;
using namespace MyDirectX;

void TextureRenderer::Initialize(DXGameObject * gameObject)
{
	mDXManager = gameObject->GetDXManager();
	mDXCamera = gameObject->GetDXCamera();
	mDeviceContext = mDXManager->GetDeviceContext();
	mGameObject = gameObject;
	mDXFactory = mDXManager->GetDXFactory();
	mDevice = mDXManager->GetDevice();
	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mDefaultColor.r = 1;
	mDefaultColor.g = 1;
	mDefaultColor.b = 1;
	mDefaultColor.a = 1;
	mColor = mDefaultColor;
	mHitColor.SetBlack();

	mRenderData->type = RenderingObjectType::Texture;
	mRenderData->topology = mTopology;
	mRenderData->stride = sizeof(TEXTURE_VERTEX_DATA);
	mRenderData->offset = 0;
	mRenderData->isAlphaBlend = mIsAlphaBlending;
	mRenderData->renderColor = XMVectorSet(mColor.r, mColor.g, mColor.b, mColor.a);
}

void TextureRenderer::LoadTexture(wchar_t * fileName)
{
	//テクスチャの読み込み
	CoInitialize(NULL);
	mRenderData->textureData = mDXManager->GetDXRenderDataPool()->GetTexture(fileName);
	CoUninitialize();
}

void TextureRenderer::Render()
{
	mRenderData->renderMatrix = mDXCamera->GetDXCameraParam(mGameObject->GetTransform());
	mDXManager->SetRenderData(mRenderData.get());
}

void TextureRenderer::Exit()
{
}
