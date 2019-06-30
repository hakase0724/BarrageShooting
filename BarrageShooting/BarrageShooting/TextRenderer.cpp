#include "stdafx.h"
#include "TextRenderer.h"

using namespace DirectX;
using namespace MyDirectX;

void TextRenderer::Initialize(DXGameObject * gameObject)
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
	CreateMesh<TextureMesh>();
	CreateShader<TextureShader>();

	mRenderData->type = RenderingObjectType::Text;
	mRenderData->topology = mTopology;
	mRenderData->stride = sizeof(TEXTURE_VERTEX_DATA);
	mRenderData->offset = 0;
	mRenderData->isAlphaBlend = mIsAlphaBlending;
	mRenderData->renderColor = XMVectorSet(mColor.r, mColor.g, mColor.b, mColor.a);
}

void TextRenderer::CreateText(const wchar_t * text,float xOffset, float yOffset, WCHAR * fontName)
{
	//constを外す
	wchar_t* w = const_cast<wchar_t*>(text);
	//オブジェクトプールへアクセス
	auto pool = mDXManager->GetDXRenderDataPool();
	mRenderData->textureData = pool->GetFontTexture(w, fontName);
	//文字を原点からずらす
	mTransform = *mGameObject->GetTransform();
	mTransform.Position.x += xOffset;
	mTransform.Position.y += yOffset;
}

void TextRenderer::Render()
{
	mRenderData->renderMatrix = mDXCamera->GetDXCameraParam(&mTransform);
	mDXManager->SetRenderData(mRenderData.get());
}

void TextRenderer::Exit()
{
}
