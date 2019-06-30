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
		//パイプラインの解釈の仕方を変更する
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
		//メッシュを作る
		template <typename T>
		void CreateMesh();
		//シェーダーを作る
		template <typename T>
		void CreateShader();
	protected:
		//描画色
		COLOR mColor;
		COLOR mDefaultColor;
		COLOR mHitColor;
		//赤
		float r = 0.0f;
		//緑
		float g = 0.0f;
		//青
		float b = 0.0f;
		//透明度
		float a = 0.0f;
		//自身が保持するデータ
		DXManager* mDXManager = nullptr;
		DXCamera* mDXCamera = nullptr;
		DXFactory* mDXFactory = nullptr;
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mDeviceContext = nullptr;
		//パイプラインの解釈の仕方
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		//透過処理を行うか
		bool mIsAlphaBlending = false;
		std::unique_ptr<RENDER_TEXTURE_DATA> mRenderData;
	};

	//メッシュを作る
	template<typename T>
	inline void MeshRenderer::CreateMesh()
	{
		//メッシュ情報取得
		mRenderData->meshInfo = mGameObject->GetDXManager()->GetDXRenderDataPool()->GetMesh<T>();
		if (mRenderData->meshInfo->mIsInitialize) return;
		//メッシュ情報をもとにレンダリングに必要なメッシュを作成
		mDXFactory->CreateMesh(mRenderData->meshInfo, mDevice);
		
	}
	//シェーダーを作る
	template<typename T>
	inline void MeshRenderer::CreateShader()
	{
		//シェーダー情報取得
		mRenderData->shaderInfo = mGameObject->GetDXManager()->GetDXRenderDataPool()->GetShader<T>();
		if (mRenderData->shaderInfo->mIsInitialize) return;
		//シェーダー情報をもとにシェーダーを作成
		mDXFactory->CreateShader(mRenderData->shaderInfo, mDevice, nullptr);
	}
}


