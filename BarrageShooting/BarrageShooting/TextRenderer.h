#pragma once
#include "MeshRenderer.h"

namespace MyDirectX
{
	class TextRenderer :public MeshRenderer
	{
	public:
		TextRenderer() {};
		~TextRenderer() {};
		virtual void Initialize(DXGameObject* gameObject) override;
		/*
		text = 表示文字
		xOffset = x方向のオフセット
		yOffset = y方向のオフセット
		fontName = フォント名　デフォルトMS P明朝
		*/
		void CreateText
		(
			const wchar_t* text,
			float xOffset = 0.0f,
			float yOffset = 0.0f,
			WCHAR* fontName = (WCHAR*)"ＭＳ Ｐ明朝"		
		);
		virtual void Render() override;
		virtual void Exit() override;
	private:
		TRANSFORM mTransform;
	};

}

