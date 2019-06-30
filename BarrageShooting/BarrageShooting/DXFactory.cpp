#include "stdafx.h"
#include "DXFactory.h"

using namespace MyDirectX;

DXFactory::DXFactory()
{
	mMeshFactory = std::make_unique<MeshFactory>();
	mShaderFactory = std::make_unique<ShaderFactory>();
}

void DXFactory::CreateMesh(MeshInfo * info, ID3D11Device * device)
{
	mMeshFactory->Create(info, device);
	info->mIsInitialize = true;
}

void DXFactory::CreateShader(ShaderInfo * info, ID3D11Device * device, ID3D11Buffer ** constantBuffer)
{
	mShaderFactory->Create(info, device, nullptr);
	info->mIsInitialize = true;
}
