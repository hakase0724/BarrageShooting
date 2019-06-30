#pragma once
#include "MeshInfo.h"
#include <d3d11.h>

namespace MyDirectX 
{
	class MeshFactory
	{
	public:
		MeshFactory() {};
		~MeshFactory() {};
		void Create(MeshInfo* info,ID3D11Device* device);
	private:
		void CreateVertex(MeshInfo* info, ID3D11Device* device);
		void CreateIndex(MeshInfo* info, ID3D11Device* device);
	};
}



