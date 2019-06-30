#pragma once
#include "ShaderInfo.h"
#include <d3d11.h>
namespace MyDirectX
{
	class ShaderFactory
	{
	public:
		ShaderFactory() {};
		~ShaderFactory() {};
		void Create(ShaderInfo * info,ID3D11Device * device,ID3D11Buffer ** constantBuffer);
	private:
		void CreateShader(ShaderInfo * info, ID3D11Device * device);
		void CreateRasterizerState(ShaderInfo * info, ID3D11Device * device);
		void CreateConstantBuffer(ShaderInfo * info, ID3D11Device * device, ID3D11Buffer ** constantBuffer);
	};
}



