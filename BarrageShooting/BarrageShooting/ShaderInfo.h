#pragma once
#include "MyStructs.h"

namespace MyDirectX 
{
	class ShaderInfo
	{
	public:
		ShaderInfo() {};
		virtual ~ShaderInfo() {};
		bool mIsInitialize = false;
		SHADER_DATA* shader = nullptr;
		virtual UINT GetSize() = 0;
		ComPtr<ID3D11VertexShader> vertexShader = nullptr;
		ComPtr<ID3D11PixelShader> pixelShader = nullptr;
		ComPtr<ID3D11InputLayout> inputLayout = nullptr;
		ComPtr<ID3D11RasterizerState> rasterizeState = nullptr;
	};
}


