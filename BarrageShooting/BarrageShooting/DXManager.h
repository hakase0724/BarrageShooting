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
		//�������Ă��郊�\�[�X�̉��
		void ResourceRelese();
		//������
		HRESULT InitDX11(HWND hwnd);
		//�I��
		void ExitDX11();
		//�`��J�n�O�̃o�b�t�@�N���A
		void BeginScene(float r,float g, float b,float a);
		//�`�揈�����I��������̉�ʂ����[�U�[�Ɍ�����
		void EndScene();
		//DirectX�̉��z�h���C�o�[�@������������ԑ厖�Ȃ���
		ID3D11Device* GetDevice() const { return mDevice.Get(); }
		//�����_�����O�ɂ��ĐF�X�ݒ�ł�����̂炵��
		ID3D11DeviceContext* GetDeviceContext() const { return mDeviceContext.Get(); }		
		//�[�x�o�b�t�@�̏��
		ID3D11DepthStencilState* GetDepthStencilState() const { return mDepthStencilState.Get(); }
		void OMSetBlendState(bool blendFlg);
		//�ێ����Ă���@�\�̃|�C���^��Ԃ�
		DXInput* GetDXInput() const { return mDXInput.get(); }
		DXCamera* GetDXCamera() const { return mDXCamera.get(); }
		DXFactory* GetDXFactory() const { return mDXFactory.get(); }
		DXRenderDataPool* GetDXRenderDataPool() const { return mDXRenderDataPool.get(); }
		ID3D11SamplerState** GetAddressOfTextureSampler() { return mTextureSamplerState.GetAddressOf(); }
		ID3D11SamplerState** GetAddressOfTextSampler() { return mTextSamplerState.GetAddressOf(); }
		void SetRenderData(RENDER_TEXTURE_DATA* renderData) { mDXRenderManager->PushRenderData(renderData); }
		void Render() { mDXRenderManager->Render(); }
	private:
		//DirectX�̉��z�h���C�o�[
		ComPtr<ID3D11Device> mDevice;
		//�����_�����O�ɂ��ĐF�X�ݒ�ł�����̂炵��
		ComPtr<ID3D11DeviceContext> mDeviceContext;
		//�`�揈���ɕK�v�ȏ�񂪓���o�b�t�@�[�炵��
		ComPtr<IDXGISwapChain> mSwapChain;
		//�����_�����O���Ɏg����T�u���\�[�X����
		ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		//�[�x�o�b�t�@�̃r���[���
		ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		//�[�x�o�b�t�@�̏��
		ComPtr<ID3D11DepthStencilState> mDepthStencilState;
#if _DEBUG
		//�f�o�b�O�p
		ComPtr<ID3D11Debug> mD3dDebug;
#endif
		//�A���t�@�u�����h�p�̃X�e�[�g
		ComPtr<ID3D11BlendState> mBlendState;
		//�e�N�X�`���p�̃T���v���[�X�e�[�g
		ComPtr<ID3D11SamplerState> mTextureSamplerState;
		//�e�L�X�g�p�̃T���v���[�X�e�[�g
		ComPtr<ID3D11SamplerState> mTextSamplerState;
		//�ێ����Ă���@�\
		std::unique_ptr<DXInput> mDXInput;
		std::unique_ptr<DXCamera> mDXCamera;
		std::unique_ptr<DXFactory> mDXFactory;
		std::unique_ptr<DXRenderDataPool> mDXRenderDataPool;
		std::unique_ptr<DXRenderManager> mDXRenderManager;
	};
}


