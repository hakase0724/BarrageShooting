#include "stdafx.h"
#include "DXCamera.h"
#include "CommonValues.h"

using namespace DirectX;
using namespace MyDirectX;

DXCamera::DXCamera(CAMERA_PARAM * param)
{
	mParam = *param;
	//スクリーン座標を取得するための行列作成
	XMVECTOR one = XMVectorSet(cWidth / 2.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR two = XMVectorSet(0.0f, -cHeight / 2.0f, 0.0f, 0.0f);
	XMVECTOR three = XMVectorSet(cWidth / 2.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR four = XMVectorSet(cWidth / 2.0f, cHeight / 2.0f, 0.0f, 1.0f);
	mMatrix = XMMATRIX(one, two, three, four);
	//カメラの位置、上方向、および焦点を使用して、左手座標系のビュー行列を作成
	mProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mParam.mRatio, 0.1f, 110.0f);
	//視野に基づいて、左手座標系のパースペクティブ射影行列を作成
	mView = XMMatrixLookAtLH(mParam.mPos, mParam.mLookup, mParam.mUp);
}

XMMATRIX DXCamera::GetDXCameraParam(TRANSFORM* transform)
{
	//x軸中心回転
	XMMATRIX worldX = XMMatrixRotationX(ToRadian(transform->Rotation.y));
	//y軸中心回転
	XMMATRIX worldY = XMMatrixRotationY(ToRadian(transform->Rotation.x));
	//z軸中心回転
	XMMATRIX worldZ = XMMatrixRotationZ(ToRadian(transform->Rotation.z));
	//移動計算
	XMMATRIX move = XMMatrixTranslation(transform->Position.x,transform->Position.y,transform->Position.z);
	//スケール計算
	XMMATRIX scale = XMMatrixScalingFromVector(XMVectorSet(transform->Scale.x, transform->Scale.y, transform->Scale.z, 0.0f));
	DirectX::XMMATRIX matrix = worldX * worldY * worldZ;
	matrix.r[0].m128_f32[0] *= transform->Scale.x;
	matrix.r[0].m128_f32[1] *= transform->Scale.x;
	matrix.r[0].m128_f32[2] *= transform->Scale.x;
	matrix.r[0].m128_f32[3] = 0.0f;
	matrix.r[1].m128_f32[0] *= transform->Scale.y;
	matrix.r[1].m128_f32[1] *= transform->Scale.y;
	matrix.r[1].m128_f32[2] *= transform->Scale.y;
	matrix.r[1].m128_f32[3] = 0.0f;
	matrix.r[2].m128_f32[0] *= transform->Scale.z;
	matrix.r[2].m128_f32[1] *= transform->Scale.z;
	matrix.r[2].m128_f32[2] *= transform->Scale.z;
	matrix.r[2].m128_f32[3] = 0.0f;
	matrix.r[3].m128_f32[0] = transform->Position.x;
	matrix.r[3].m128_f32[1] = transform->Position.y;
	matrix.r[3].m128_f32[2] = transform->Position.z;
	matrix.r[3].m128_f32[3] = 1.0f;
	//転置行列を計算
	return XMMatrixTranspose(matrix * mView * mProj);
}

XMMATRIX DXCamera::GetWorld(TRANSFORM* transform)
{
	//x軸中心回転
	XMMATRIX worldX = XMMatrixRotationX(transform->Rotation.y);
	//y軸中心回転
	XMMATRIX worldY = XMMatrixRotationY(transform->Rotation.x);
	//z軸中心回転
	XMMATRIX worldZ = XMMatrixRotationZ(transform->Rotation.z);
	//移動計算
	XMMATRIX move = XMMatrixTranslation(transform->Position.x, transform->Position.y, transform->Position.z);
	//スケール計算
	XMMATRIX scale = XMMatrixScalingFromVector(XMVectorSet(transform->Scale.x, transform->Scale.y, transform->Scale.z, 0.0f));

	return XMMatrixTranspose(scale * worldX * worldY * worldZ * move);
}

XMVECTOR DXCamera::WorldToScreenPoint(TRANSFORM* transform)
{
	auto vector = XMVectorSet(transform->Position.x, transform->Position.y, transform->Position.z, 1.0f);
	auto screen = XMVector3Transform(vector, mMatrix);
	return screen;
}

//画面比率とカメラのパラメータを設定する
void DXCamera::SetEyeParamWithRatio(XMVECTOR pos, XMVECTOR lookup, XMVECTOR up, float ratio)
{
	mParam.mPos = pos;
	mParam.mLookup = lookup;
	mParam.mUp = up;
	mParam.mRatio = ratio;
	//カメラの位置、上方向、および焦点を使用して、左手座標系のビュー行列を作成
	mProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mParam.mRatio, 0.1f, 110.0f);
	//視野に基づいて、左手座標系のパースペクティブ射影行列を作成
	mView = XMMatrixLookAtLH(mParam.mPos, mParam.mLookup, mParam.mUp);
}
//カメラを回す
void DXCamera::SetRotation(float x, float y)
{
	mParam.mXRote = x;
	mParam.mYRote = y;
}
//カメラの位置を動かす
void DXCamera::SetPos(float x, float y, float z)
{
	mParam.mPos += XMVectorSet(x, y, z,0.0f);
}
