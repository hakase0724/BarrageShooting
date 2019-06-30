#pragma once
#include <DirectXMath.h>
#include "MyStructs.h"
#include "AlignedAllocationPolicy.h"
namespace MyDirectX
{
	class DXCamera:public AlignedAllocationPolicy<16>
	{
	public:
		DXCamera(CAMERA_PARAM *param);
		~DXCamera() {};
		DirectX::XMMATRIX GetDXCameraParam(TRANSFORM* transform);
		CAMERA_PARAM GetCameraParam() const { return mParam;}
		DirectX::XMMATRIX GetWorld(TRANSFORM* transform);
		DirectX::XMVECTOR WorldToScreenPoint(TRANSFORM* transform);
		void SetEyeParamWithRatio(DirectX::XMVECTOR pos, DirectX::XMVECTOR lookup, DirectX::XMVECTOR up,float ratio);
		void SetRotation(float x, float y );
		void SetPos(float x = 0.0f, float y = 0.0f,float z = 0.0f);
	private:
		//度数法表記の角度を弧度法表記の角度に変換する
		float ToRadian(float angle) { return angle * (float)(acos(-1.0f) / 180.0f); }
		CAMERA_PARAM mParam;	
		DirectX::XMMATRIX mMatrix;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProj;
	};
}


