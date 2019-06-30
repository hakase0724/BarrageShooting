#pragma once
#include "EnemyBase.h"

namespace MyDirectX
{
	class NormalEnemy :public EnemyBase
	{
	public:
		NormalEnemy() {};
		~NormalEnemy() {};
		virtual void Initialize(DXGameObject* gameObject) override { EnemyBase::Initialize(gameObject); }
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void OnDisable() override;
		void SetColor(COLOR color) {}
		void SetBulletTexture(const wchar_t* path) { mBulletTexturePath = path; }
	private:
		COLOR mColor;
		std::wstring mBulletTexturePath = L"";
	};
}


