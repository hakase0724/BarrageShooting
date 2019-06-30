#pragma once
#include <DirectXMath.h>
#include "MyStructs.h"
#include <vector>
#include <map>
#include <set>
namespace MyDirectX
{
	class DXManager;
	struct PerInstanceData
	{
		DirectX::XMMATRIX renderMatrix;
		DirectX::XMVECTOR renderColor;
		PerInstanceData(DirectX::XMMATRIX matrix,DirectX::XMVECTOR color)
		{
			renderMatrix = matrix;
			renderColor = color;
		}
	};
	
	class DXRenderManager
	{
	public:
		DXRenderManager(DXManager* manager);
		~DXRenderManager();
		void Render();
		void PushRenderData(RENDER_TEXTURE_DATA* renderData);
	private:
		DXManager* mManager;
		ID3D11DeviceContext* mDeviceContext;
		ID3D11Device* mDevice;
		ComPtr<ID3D11Buffer> mPerInstanceBuffer;
		ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
		std::map <std::wstring, std::vector<RENDER_TEXTURE_DATA*>> mTextureRenderData;
		std::set<std::wstring> mFileNames;
		D3D11_MAPPED_SUBRESOURCE mMapResource;
		PerInstanceData* mInstanceData = nullptr;
	};
}


