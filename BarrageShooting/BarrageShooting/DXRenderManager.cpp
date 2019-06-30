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

	//�C���X�^���X�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_instance;
	bd_instance.Usage = D3D11_USAGE_DYNAMIC;
	bd_instance.ByteWidth = sizeof(PerInstanceData) * 500;
	bd_instance.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd_instance.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_instance.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd_instance.StructureByteStride = sizeof(PerInstanceData);
	mDevice->CreateBuffer(&bd_instance, NULL, &mPerInstanceBuffer);
	//�C���X�^���X�p�̃��\�[�X�r���[���쐬
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
	// �p�����[�^�̎󂯓n��
	for (auto fileName : mFileNames)
	{
		auto textureVector = mTextureRenderData[fileName];
		int instanceCount = textureVector.size();
		//�f�[�^�������Ă��Ȃ���΂��̃t���[���ł͕`�悵�Ȃ��e�N�X�`���ł���
		if (instanceCount < 1) continue;
		//�����Ă���f�[�^�͑S�ē����ɕ`��ł�����̂̂��ߐ擪�̃f�[�^���g�p����
		auto render = textureVector.front();
		mDeviceContext->IASetVertexBuffers(0, 1, render->meshInfo->vertexBuffer.GetAddressOf(), &render->stride, &render->offset);
		//�C���f�b�N�X�o�b�t�@�[�̐ݒ�
		mDeviceContext->IASetIndexBuffer(render->meshInfo->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, render->offset);
		//���̓��C�A�E�g�ݒ�
		mDeviceContext->IASetInputLayout(render->shaderInfo->inputLayout.Get());
		//���_���̉��߂̎d����ݒ�
		mDeviceContext->IASetPrimitiveTopology(render->topology);
		//�`�悷�鐔���z��̃f�[�^�̐�
		mDeviceContext->VSSetShader(render->shaderInfo->vertexShader.Get(), NULL, 0);
		mDeviceContext->PSSetShader(render->shaderInfo->pixelShader.Get(), NULL, 0);
		//�e�N�X�`�����e�L�X�g���ŃT���v���[���Ⴄ
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
			//�s������Z�b�g
			mInstanceData[i].renderMatrix = textureVector[i]->renderMatrix;
			//�F�����Z�b�g
			mInstanceData[i].renderColor = textureVector[i]->renderColor;
		}
		mDeviceContext->Unmap(mPerInstanceBuffer.Get(), 0);
		ID3D11ShaderResourceView* srv[] = { mShaderResourceView.Get() ,render->textureData->shaderView };
		mDeviceContext->PSSetShaderResources(0, 2, srv);
		mManager->OMSetBlendState(render->isAlphaBlend);
		// �`����s
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
