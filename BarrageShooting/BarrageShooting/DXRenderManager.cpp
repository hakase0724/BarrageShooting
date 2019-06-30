#include "stdafx.h"
#include "DXRenderManager.h"
#include <algorithm>
#include <functional>
#include "DXManager.h"
#include "FuctionCountTimer.h"

using namespace MyDirectX;
DXRenderManager::DXRenderManager(DXManager* manager)
{
	mManager = manager;
	mDevice = mManager->GetDevice();
	mDeviceContext = mManager->GetDeviceContext();

	//インスタンスデータ用バッファの設定
	D3D11_BUFFER_DESC bd_instance;
	bd_instance.Usage = D3D11_USAGE_DYNAMIC;
	bd_instance.ByteWidth = sizeof(PerInstanceData) * 500;
	bd_instance.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd_instance.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_instance.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd_instance.StructureByteStride = sizeof(PerInstanceData);
	mDevice->CreateBuffer(&bd_instance, NULL, &mPerInstanceBuffer);
	//インスタンス用のリソースビューを作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.NumElements = 500;
	mDevice->CreateShaderResourceView(mPerInstanceBuffer.Get(), &srvDesc, &mShaderResourceView);
	mDeviceContext->VSSetShaderResources(0, 1, mShaderResourceView.GetAddressOf());
	mDeviceContext->OMSetDepthStencilState(mManager->GetDepthStencilState(), 0);
}

DXRenderManager::~DXRenderManager()
{
	mTextureRenderData.clear();
	mFileNames.clear();
}

void MyDirectX::DXRenderManager::Render()
{
	// パラメータの受け渡し
	for (auto fileName : mFileNames)
	{
		auto textureVector = mTextureRenderData[fileName];
		int instanceCount = textureVector.size();
		//データが入っていなければこのフレームでは描画しないテクスチャである
		if (instanceCount < 1) continue;
		//入っているデータは全て同時に描画できるもののため先頭のデータを使用する
		auto render = textureVector.front();
		mDeviceContext->IASetVertexBuffers(0, 1, render->meshInfo->vertexBuffer.GetAddressOf(), &render->stride, &render->offset);
		//インデックスバッファーの設定
		mDeviceContext->IASetIndexBuffer(render->meshInfo->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, render->offset);
		//入力レイアウト設定
		mDeviceContext->IASetInputLayout(render->shaderInfo->inputLayout.Get());
		//頂点情報の解釈の仕方を設定
		mDeviceContext->IASetPrimitiveTopology(render->topology);
		//描画する数＝配列のデータの数
		mDeviceContext->VSSetShader(render->shaderInfo->vertexShader.Get(), NULL, 0);
		mDeviceContext->PSSetShader(render->shaderInfo->pixelShader.Get(), NULL, 0);
		//テクスチャかテキストかでサンプラーが違う
		switch (render->type)
		{
		case RenderingObjectType::Texture:
			mDeviceContext->PSSetSamplers(0, 1, mManager->GetAddressOfTextureSampler());
			break;
		case RenderingObjectType::Text:
			mDeviceContext->PSSetSamplers(0, 1, mManager->GetAddressOfTextSampler());
			break;
		default:
			break;
		}
		mDeviceContext->Map(mPerInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mMapResource);
		mInstanceData = (PerInstanceData*)(mMapResource.pData);
		for (int i = 0; i < instanceCount; i++)
		{
			//行列情報をセット
			mInstanceData[i].renderMatrix = textureVector[i]->renderMatrix;
			//色情報をセット
			mInstanceData[i].renderColor = textureVector[i]->renderColor;
		}
		mDeviceContext->Unmap(mPerInstanceBuffer.Get(), 0);
		ID3D11ShaderResourceView* srv[] = { mShaderResourceView.Get() ,render->textureData->shaderView };
		mDeviceContext->PSSetShaderResources(0, 2, srv);
		mManager->OMSetBlendState(render->isAlphaBlend);
		// 描画実行
		mDeviceContext->DrawIndexedInstanced(render->meshInfo->indexNum, instanceCount, 0, 0, 0);
	}
	mTextureRenderData.clear();
	mFileNames.clear();
}

void MyDirectX::DXRenderManager::PushRenderData(RENDER_TEXTURE_DATA* renderData)
{
	mTextureRenderData[renderData->textureData->fileName].push_back(renderData);
	mFileNames.insert(renderData->textureData->fileName);
}
