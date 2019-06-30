#pragma once
#include <d3d11.h>
#include <memory>
#include "DXInput.h"
#include "DXCamera.h"
#include <DirectXMath.h>
#include "DXFactory.h"
#include "MyStructs.h"
#include "DXRenderDataPool.h"
#include "CommonValues.h"
#include <dxgidebug.h>
#include "AlignedAllocationPolicy.h"
#include "DXRenderManager.h"

namespace MyDirectX
{
	class DXManager:public AlignedAllocationPolicy<16>
	{
	public:
		DXManager(HWND hwnd);
		~DXManager();
		//所持しているリソースの解放
		void ResourceRelese();
		//初期化
		HRESULT InitDX11(HWND hwnd);
		//終了
		void ExitDX11();
		//描画開始前のバッファクリア
		void BeginScene(float r,float g, float b,float a);
		//描画処理が終了した後の画面をユーザーに見せる
		void EndScene();
		//DirectXの仮想ドライバー　多分こいつが一番大事なもの
		ID3D11Device* GetDevice() const { return mDevice.Get(); }
		//レンダリングについて色々設定できるものらしい
		ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext.Get(); }		
		//深度バッファの状態
		ID3D11DepthStencilState* GetDepthStencilState() const { return mDepthStencilState.Get(); }
		void OMSetBlendState(bool blendFlg);
		//保持している機能のポインタを返す
		DXInput* GetDXInput() const { return mDXInput.get(); }
		DXCamera* GetDXCamera() const { return mDXCamera.get(); }
		DXFactory* GetDXFactory() const { return mDXFactory.get(); }
		DXRenderDataPool* GetDXRenderDataPool() const { return mDXRenderDataPool.get(); }
		ID3D11SamplerState** GetAddressOfTextureSampler() { return mTextureSamplerState.GetAddressOf(); }
		ID3D11SamplerState** GetAddressOfTextSampler() { return mTextSamplerState.GetAddressOf(); }
		void SetRenderData(RENDER_TEXTURE_DATA* renderData) { mDXRenderManager->PushRenderData(renderData); }
		void Render() { mDXRenderManager->Render(); }
	private:
		//DirectXの仮想ドライバー
		ComPtr<ID3D11Device> mDevice;
		//レンダリングについて色々設定できるものらしい
		ComPtr<ID3D11DeviceContext> mDeviceContext;
		//描画処理に必要な情報が入るバッファーらしい
		ComPtr<IDXGISwapChain> mSwapChain;
		//レンダリング時に使えるサブリソース識別
		ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		//深度バッファのビュー情報
		ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		//深度バッファの状態
		ComPtr<ID3D11DepthStencilState> mDepthStencilState;
#if _DEBUG
		//デバッグ用
		ComPtr<ID3D11Debug> mD3dDebug;
#endif
		//アルファブレンド用のステート
		ComPtr<ID3D11BlendState> mBlendState;
		//テクスチャ用のサンプラーステート
		ComPtr<ID3D11SamplerState> mTextureSamplerState;
		//テキスト用のサンプラーステート
		ComPtr<ID3D11SamplerState> mTextSamplerState;
		//保持している機能
		std::unique_ptr<DXInput> mDXInput;
		std::unique_ptr<DXCamera> mDXCamera;
		std::unique_ptr<DXFactory> mDXFactory;
		std::unique_ptr<DXRenderDataPool> mDXRenderDataPool;
		std::unique_ptr<DXRenderManager> mDXRenderManager;
	};
}


