#pragma once
#include <d3d11.h>
#include <typeinfo>
#include "DXManager.h"
#include "IComponent.h"
#include "DXGameObject.h"
#include "MeshAndShader.h"
#include "DXRenderDataPool.h"

namespace MyDirectX
{
	class MeshRenderer :public Component,public AlignedAllocationPolicy<16>
	{
	public:
		MeshRenderer() 
		{
			mRenderData = std::make_unique<RENDER_TEXTURE_DATA>();
		};
		~MeshRenderer() {};
		//�p�C�v���C���̉��߂̎d����ύX����
		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { mTopology = topology; }
		void SetDefaultColor(float r, float g, float b, float a);
		void SetDefaultColor(COLOR color) { SetDefaultColor(color.r, color.g, color.b, color.a); };
		void SetColor(float r, float g, float b, float a);
		void SetColor(COLOR color) { SetColor(color.r, color.g, color.b, color.a); }
		void SetColor();
		void SetHitColor(float r, float g, float b, float a);
		void SetHitColor(COLOR color) { SetHitColor(color.r, color.g, color.b, color.a); }
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Exit() override;
		virtual void OnCollisionEnter2D(Collider2D* col) override;
		virtual void OnCollisionExit2D(Collider2D* col) override;
		void SetAlphaBlendingFlg(bool flg);
		//���b�V�������
		template <typename T>
		void CreateMesh();
		//�V�F�[�_�[�����
		template <typename T>
		void CreateShader();
	protected:
		//�`��F
		COLOR mColor;
		COLOR mDefaultColor;
		COLOR mHitColor;
		//��
		float r = 0.0f;
		//��
		float g = 0.0f;
		//��
		float b = 0.0f;
		//�����x
		float a = 0.0f;
		//���g���ێ�����f�[�^
		DXManager* mDXManager = nullptr;
		DXCamera* mDXCamera = nullptr;
		DXFactory* mDXFactory = nullptr;
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mDeviceContext = nullptr;
		//�p�C�v���C���̉��߂̎d��
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		//���ߏ������s����
		bool mIsAlphaBlending = false;
		std::unique_ptr<RENDER_TEXTURE_DATA> mRenderData;
	};

	//���b�V�������
	template<typename T>
	inline void MeshRenderer::CreateMesh()
	{
		//���b�V�����擾
		mRenderData->meshInfo = mGameObject->GetDXManager()->GetDXRenderDataPool()->GetMesh<T>();
		if (mRenderData->meshInfo->mIsInitialize) return;
		//���b�V���������ƂɃ����_�����O�ɕK�v�ȃ��b�V�����쐬
		mDXFactory->CreateMesh(mRenderData->meshInfo, mDevice);
		
	}
	//�V�F�[�_�[�����
	template<typename T>
	inline void MeshRenderer::CreateShader()
	{
		//�V�F�[�_�[���擾
		mRenderData->shaderInfo = mGameObject->GetDXManager()->GetDXRenderDataPool()->GetShader<T>();
		if (mRenderData->shaderInfo->mIsInitialize) return;
		//�V�F�[�_�[�������ƂɃV�F�[�_�[���쐬
		mDXFactory->CreateShader(mRenderData->shaderInfo, mDevice, nullptr);
	}
}


