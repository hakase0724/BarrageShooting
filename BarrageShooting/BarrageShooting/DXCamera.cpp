#include "stdafx.h"
#include "DXCamera.h"
#include "CommonValues.h"

using namespace DirectX;
using namespace MyDirectX;

DXCamera::DXCamera(CAMERA_PARAM * param)
{
	mParam = *param;
	//�X�N���[�����W���擾���邽�߂̍s��쐬
	XMVECTOR one = XMVectorSet(cWidth / 2.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR two = XMVectorSet(0.0f, -cHeight / 2.0f, 0.0f, 0.0f);
	XMVECTOR three = XMVectorSet(cWidth / 2.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR four = XMVectorSet(cWidth / 2.0f, cHeight / 2.0f, 0.0f, 1.0f);
	mMatrix = XMMATRIX(one, two, three, four);
	//�J�����̈ʒu�A������A����яœ_���g�p���āA������W�n�̃r���[�s����쐬
	mProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mParam.mRatio, 0.1f, 110.0f);
	//����Ɋ�Â��āA������W�n�̃p�[�X�y�N�e�B�u�ˉe�s����쐬
	mView = XMMatrixLookAtLH(mParam.mPos, mParam.mLookup, mParam.mUp);
}

XMMATRIX DXCamera::GetDXCameraParam(TRANSFORM* transform)
{
	//x�����S��]
	XMMATRIX worldX = XMMatrixRotationX(ToRadian(transform->Rotation.y));
	//y�����S��]
	XMMATRIX worldY = XMMatrixRotationY(ToRadian(transform->Rotation.x));
	//z�����S��]
	XMMATRIX worldZ = XMMatrixRotationZ(ToRadian(transform->Rotation.z));
	//�ړ��v�Z
	XMMATRIX move = XMMatrixTranslation(transform->Position.x,transform->Position.y,transform->Position.z);
	//�X�P�[���v�Z
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
	//�]�u�s����v�Z
	return XMMatrixTranspose(matrix * mView * mProj);
}

XMMATRIX DXCamera::GetWorld(TRANSFORM* transform)
{
	//x�����S��]
	XMMATRIX worldX = XMMatrixRotationX(transform->Rotation.y);
	//y�����S��]
	XMMATRIX worldY = XMMatrixRotationY(transform->Rotation.x);
	//z�����S��]
	XMMATRIX worldZ = XMMatrixRotationZ(transform->Rotation.z);
	//�ړ��v�Z
	XMMATRIX move = XMMatrixTranslation(transform->Position.x, transform->Position.y, transform->Position.z);
	//�X�P�[���v�Z
	XMMATRIX scale = XMMatrixScalingFromVector(XMVectorSet(transform->Scale.x, transform->Scale.y, transform->Scale.z, 0.0f));

	return XMMatrixTranspose(scale * worldX * worldY * worldZ * move);
}

XMVECTOR DXCamera::WorldToScreenPoint(TRANSFORM* transform)
{
	auto vector = XMVectorSet(transform->Position.x, transform->Position.y, transform->Position.z, 1.0f);
	auto screen = XMVector3Transform(vector, mMatrix);
	return screen;
}

//��ʔ䗦�ƃJ�����̃p�����[�^��ݒ肷��
void DXCamera::SetEyeParamWithRatio(XMVECTOR pos, XMVECTOR lookup, XMVECTOR up, float ratio)
{
	mParam.mPos = pos;
	mParam.mLookup = lookup;
	mParam.mUp = up;
	mParam.mRatio = ratio;
	//�J�����̈ʒu�A������A����яœ_���g�p���āA������W�n�̃r���[�s����쐬
	mProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mParam.mRatio, 0.1f, 110.0f);
	//����Ɋ�Â��āA������W�n�̃p�[�X�y�N�e�B�u�ˉe�s����쐬
	mView = XMMatrixLookAtLH(mParam.mPos, mParam.mLookup, mParam.mUp);
}
//�J��������
void DXCamera::SetRotation(float x, float y)
{
	mParam.mXRote = x;
	mParam.mYRote = y;
}
//�J�����̈ʒu�𓮂���
void DXCamera::SetPos(float x, float y, float z)
{
	mParam.mPos += XMVectorSet(x, y, z,0.0f);
}
