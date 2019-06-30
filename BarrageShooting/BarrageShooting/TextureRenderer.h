#pragma once
#include "MeshRenderer.h"

namespace MyDirectX
{
	class TextureRenderer :public MeshRenderer
	{
	public:
		TextureRenderer() {};
		~TextureRenderer() {};
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void LoadTexture(wchar_t* fileName);
		virtual void Render() override;
		virtual void Exit() override;
	};

}

