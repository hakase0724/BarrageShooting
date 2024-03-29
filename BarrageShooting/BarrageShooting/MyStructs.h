#pragma once
#include <string>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Primitive2D.h"
#include "MyEnums.h"
#include "AlignedAllocationPolicy.h"
#include <wrl/client.h>
using namespace Microsoft::WRL;
namespace MyDirectX
{
	class DXGameObject;
	class Bullet;
	class MeshInfo;
	class ShaderInfo;

	//テクスチャ情報を保持
	struct TEXTURE_DATA
	{
		std::wstring fileName;
		ID3D11Resource* texture;
		ID3D11ShaderResourceView* shaderView;
		~TEXTURE_DATA() 
		{
			if (texture) 
			{
				texture->Release();
				texture = nullptr;
			}
			if (shaderView) 
			{
				shaderView->Release();
				shaderView = nullptr;
			}
		}
	};

	struct RENDER_TEXTURE_DATA :public AlignedAllocationPolicy<16>
	{
		TEXTURE_DATA* textureData = nullptr;
		MeshInfo* meshInfo = nullptr;
		ShaderInfo* shaderInfo = nullptr;
		RenderingObjectType type;
		DirectX::XMMATRIX renderMatrix;
		DirectX::XMVECTOR renderColor;
		D3D11_PRIMITIVE_TOPOLOGY topology;
		bool isAlphaBlend = false;
		UINT stride = 0;
		UINT offset = 0;
	};
	//色情報を保持する構造体
	struct COLOR
	{
		//赤
		float r = 1.0f;
		//緑
		float g = 1.0f;
		//青
		float b = 1.0f;
		//アルファ値
		float a = 1.0f;

		void SetWhite()
		{
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
		}

		void SetBlack()
		{
			r = 0.0f;
			g = 0.0f;
			b = 0.0f;
		}

		void SetRed()
		{
			r = 1.0f;
			g = 0.0f;
			b = 0.0f;
		}

		void SetBlue()
		{
			r = 0.0f;
			g = 0.0f;
			b = 1.0f;
		}

		void SetGreen()
		{
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
	};

	//定数バッファ
	struct CONSTANT_BUFFER:public AlignedAllocationPolicy<16> 
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;
		DirectX::XMVECTOR vLightPos;
		DirectX::XMVECTOR vEyePos;
		DirectX::XMVECTOR vColor;
	};

	struct TEXTURE_CONSTANT_BUFFER
	{
		DirectX::XMMATRIX mWVP;
		DirectX::XMVECTOR mColor;
	};

	//カメラ情報
	struct CAMERA_PARAM:public AlignedAllocationPolicy<16>
	{
		DirectX::XMVECTOR mPos;
		DirectX::XMVECTOR mLookup;
		DirectX::XMVECTOR mUp;
		float mRatio;
		float mXRote;
		float mYRote;
		CAMERA_PARAM()
		{
			mPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			mLookup = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			mUp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			mRatio = 0.0f;
			mXRote = 0.0f;
			mYRote = 0.0f;
		}

		CAMERA_PARAM(DirectX::XMVECTOR pos, DirectX::XMVECTOR lookup, DirectX::XMVECTOR up, float ratio)
		{
			mPos = pos;
			mLookup = lookup;
			mUp = up;
			mRatio = ratio;
			mXRote = 0.0f;
			mYRote = 0.0f;
		}
	};

	//オブジェクトが持つ位置回転スケール情報
	struct TRANSFORM
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Rotation;
		DirectX::XMFLOAT3 Scale;

		TRANSFORM()
		{
			Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		}

		TRANSFORM(DirectX::XMFLOAT3 pos)
		{
			Position = pos;
			Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		}

		TRANSFORM(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rote)
		{
			Position = pos;
			Rotation = rote;
			Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		}

		TRANSFORM(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rote, DirectX::XMFLOAT3 scale)
		{
			Position = pos;
			Rotation = rote;
			Scale = scale;
		}
	};

	//頂点情報構造体
	struct VERTEX_DATA
	{
		DirectX::XMFLOAT3 V;
		DirectX::XMFLOAT3 N;
		VERTEX_DATA() = default;
		VERTEX_DATA(DirectX::XMFLOAT3 v, DirectX::XMFLOAT3 n)
		{
			V = v;
			N = n;
		}
	};

	struct TEXTURE_VERTEX_DATA
	{
		DirectX::XMFLOAT3 V;
		DirectX::XMFLOAT2 T;
		TEXTURE_VERTEX_DATA() = default;
		TEXTURE_VERTEX_DATA(DirectX::XMFLOAT3 v, DirectX::XMFLOAT2 t)
		{
			V = v;
			T = t;
		}
	};

	//シェーダーに設定するデータ
	struct SHADER_DATA
	{
		LPCWSTR VertexName;
		LPCWSTR PixelName;
		LPCSTR VertexEntry;
		LPCSTR PixelEntry;
		LPCSTR VertexVersion;
		LPCSTR PixelVersion;
		D3D11_INPUT_ELEMENT_DESC* InputLayout;
		int LayoutNum;
	};

	

	//外部データ管理用構造体
	struct LOAD_FROM_CSV_DATA
	{
		std::string Name;
		DirectX::XMFLOAT3 StartPos;
		double HP;
		Tag Tag;
		int Wave;
		Category Category;
	};

	struct BULLET_SETTING_DATA
	{
		//位置情報
		TRANSFORM* pTransform;
		TRANSFORM transform;
		COLOR color;
		//弾に設定するタグ
		Tag tag;
		//x方向にかける力
		float xVectol;
		//y方向にかける力
		float yVectol;
		//読み込む画像のパス
		std::wstring texturePath;
		void ScaleRatio(float ratio)
		{
			scaleXRatio = ratio;
			scaleYRatio = ratio;
			scaleZRatio = ratio;
		}
		//自身のスケールを1とした時の弾の大きさの割合
		float scaleXRatio = 1.0f;
		float scaleYRatio = 1.0f;
		float scaleZRatio = 1.0f;
		bool isXFixed = false;
		bool isYFixed = false;
	};

}
