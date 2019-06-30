#include "stdafx.h"
#include "DXManager.h"
#include "dinput.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace MyDirectX;

DXManager::DXManager(HWND hwnd)
{
	InitDX11(hwnd);
}

DXManager::~DXManager()
{
	ExitDX11();
}

void MyDirectX::DXManager::ResourceRelese()
{
	mDXInput.reset();
	mDXFactory.reset();
	mDXCamera.reset();
	mDXRenderManager.reset();
	mDXRenderDataPool.reset();
}

//初期化
HRESULT DXManager::InitDX11(HWND hwnd)
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT flags = 0;

#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
	auto driverType = D3D_DRIVER_TYPE_HARDWARE;
	//スワップチェイン設定
	DXGI_SWAP_CHAIN_DESC sd;
	//渡したもの全てを0クリアする
	//クリアした後中身を入れずにアクセスするとエラーになることが多いため要注意らしい
	ZeroMemory(&sd,sizeof(sd));
	//バッファ数
	sd.BufferCount = 1;
	//解像度幅
	sd.BufferDesc.Width = width;
	//解像度高さ
	sd.BufferDesc.Height = height;
	//リフレッシュレートの最大有理数　変更可能な場合のみ画面のリフレッシュレートをこの値に変更する
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//リフレッシュレートの最小有理数　
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//表示フォーマット　4要素符号無し32ビット
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//バックバッファのサーフェス使用法CPUアクセスオプションを設定する　出力で使うようにする
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//出力するウィンドウのハンドル
	sd.OutputWindow = hwnd;
	//ピクセル単位のサンプリング数
	sd.SampleDesc.Count = 1;
	//イメージ品質　-1～0まで指定できる
	sd.SampleDesc.Quality = 0;
	//ウィンドウモードで起動する
	sd.Windowed = true;

	//DirectXのデバイスを作る
	hr = D3D11CreateDeviceAndSwapChain
	(NULL,driverType,NULL,flags,&fLevel,1,D3D11_SDK_VERSION,&sd,&mSwapChain,&mDevice,NULL,&mDeviceContext);
	if (FAILED(hr)) return S_FALSE;
#if _DEBUG
	mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(mD3dDebug.GetAddressOf()));
#endif
	mDXInput = std::make_unique<DXInput>(hwnd);
	//カメラを生成
	auto cParam = new CAMERA_PARAM
	(
		DirectX::XMVectorSet(0.0f, 0.0f, -3.5f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
		cWidth / cHeight
	);
	mDXCamera = std::make_unique<DXCamera>(cParam);
	delete cParam;
	//シェーダーとメッシュを作るクラス
	mDXFactory = std::make_unique<DXFactory>();

	//バックバッファの確保
	ID3D11Texture2D* back_buff;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buff);
	if (FAILED(hr)) 
	{
		ExitDX11();
		return S_FALSE;
	}

	//深度バッファの状態生成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, (sizeof(depthStencilDesc)));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);

	//レンダーターゲットビュー生成
	hr = mDevice->CreateRenderTargetView(back_buff, NULL, &mRenderTargetView);
	if (FAILED(hr))
	{
		ExitDX11();
		return S_FALSE;
	}
	if (back_buff) 
	{
		back_buff->Release();
		back_buff = nullptr;
	}

	//深度バッファで使うテクスチャ生成
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencil = 0;
	hr = mDevice->CreateTexture2D(&depthDesc, NULL, &depthStencil);

	//深度バッファのビュー情報生成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilView;
	ZeroMemory(&depthStencilView, sizeof(depthStencilView));
	depthStencilView.Format = depthDesc.Format;
	depthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilView.Texture2D.MipSlice = 0;
	depthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = mDevice->CreateDepthStencilView(depthStencil, &depthStencilView, &mDepthStencilView);
	if (depthStencil)
	{
		depthStencil->Release();
		depthStencil = nullptr;
	}

	//ビューポート設定
	D3D11_VIEWPORT view;
	view.Width = (FLOAT)width;
	view.Height = (FLOAT)height;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	mDeviceContext->RSSetViewports(1, &view);

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	mDevice->CreateBlendState(&BlendDesc, &mBlendState);

	mDXRenderDataPool = std::make_unique<DXRenderDataPool>(mDevice.Get(),mDeviceContext.Get());
	mDXRenderManager = std::make_unique<DXRenderManager>(this);	

	// シェーダ用にサンプラを作成する
	D3D11_SAMPLER_DESC textSamDesc;
	ZeroMemory(&textSamDesc, sizeof(textSamDesc));
	textSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	textSamDesc.AddressU = textSamDesc.AddressV = textSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textSamDesc.MaxAnisotropy = 1;
	textSamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	textSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	mDevice->CreateSamplerState(&textSamDesc,&mTextSamplerState);

	//サンプラーデスク作成
	D3D11_SAMPLER_DESC textureSmpDesc;
	::ZeroMemory(&textureSmpDesc, sizeof(D3D11_SAMPLER_DESC));
	textureSmpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	textureSmpDesc.AddressU = textureSmpDesc.AddressV = textureSmpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	textureSmpDesc.MipLODBias = 0.0f;
	textureSmpDesc.MaxAnisotropy = 1;
	textureSmpDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	textureSmpDesc.BorderColor[0] = 0;
	textureSmpDesc.BorderColor[1] = 0;
	textureSmpDesc.BorderColor[2] = 0;
	textureSmpDesc.BorderColor[3] = 0;
	textureSmpDesc.MinLOD = 0;
	textureSmpDesc.MaxLOD = D3D11_FLOAT32_MAX;
	mDevice->CreateSamplerState(&textureSmpDesc, &mTextureSamplerState);

	/*OutputDebugStringA("CreateDeviceDetail########################################\n");
	mD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	OutputDebugStringA("CreateDeviceSummary########################################\n");
	mD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);*/
	return S_OK;
}
//終了
void DXManager::ExitDX11()
{
	ResourceRelese();
	/*OutputDebugStringA("LastDetail########################################\n");
	mD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	OutputDebugStringA("LastSummary########################################\n");
	mD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);*/
}
//描画開始前のバッファクリア
void DXManager::BeginScene(float r, float g, float b, float a)
{
	float color[] = {r,g,b,a};
	//画面クリア
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
	//深度バッファクリア
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//描画処理が終了した後の画面をユーザーに見せる
void DXManager::EndScene()
{
	//VSync無
	mSwapChain->Present(0, 0);
}

void DXManager::OMSetBlendState(bool blendFlg)
{
	if(blendFlg)
	{
		mDeviceContext->OMSetBlendState(mBlendState.Get(), NULL, 0xffffffff);
	}
	else
	{
		mDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	}
}

