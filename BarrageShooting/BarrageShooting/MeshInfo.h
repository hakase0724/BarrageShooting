#pragma once
#include "MyStructs.h"
#include "CommonValues.h"
namespace MyDirectX 
{
	class MeshInfo
	{
	public:
		MeshInfo() {};
		virtual ~MeshInfo() {};
		bool mIsInitialize = false;
		int* indexes = nullptr;
		VERTEX_DATA* vertics = nullptr;
		TEXTURE_VERTEX_DATA* vertexInfo = nullptr;
		int indexNum = 0;
		int vertexNum = 0;
		virtual UINT GetSize() = 0;
		ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
		ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	};
}


