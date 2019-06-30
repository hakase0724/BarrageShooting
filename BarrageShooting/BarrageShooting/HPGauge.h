#pragma once
#include "IComponent.h"
#include "DXGameObject.h"

namespace MyDirectX
{
	class HPGauge :public Component
	{
	public:
		HPGauge() {};
		~HPGauge() {};
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		void SetHPViewObject(IHP* hp) { mHPViewObject = hp; }
		virtual void Update() override;
	private:
		//体力を表示するオブジェクト
		IHP* mHPViewObject = nullptr;
		//計算に使う初期値
		TRANSFORM* mTransform = nullptr;
		float mDefaultScaleX = 0.0f;
		float mDefaultPositionX = 0.0f;
		double mMaxHitPoint = 0.0;
	};
}


