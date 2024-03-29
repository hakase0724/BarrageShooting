#pragma once
#include <d3d11.h>
#include <memory>
#include "MeshFactory.h"
#include "MeshInfo.h"
#include "ShaderFactory.h"
#include "ShaderInfo.h"

namespace MyDirectX
{
	class DXFactory
	{
	public:
		DXFactory();
		~DXFactory() {};
		//メッシュを作る
		void CreateMesh(MeshInfo* info,ID3D11Device* device);
		//シェーダーを作る
		void CreateShader(ShaderInfo * info,ID3D11Device * device,ID3D11Buffer ** constantBuffer);
	private:
		//メッシュを作るクラス
		std::unique_ptr<MeshFactory> mMeshFactory;
		//シェーダーを作るクラス
		std::unique_ptr<ShaderFactory> mShaderFactory;
	};
}


