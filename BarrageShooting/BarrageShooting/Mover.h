#pragma once
#include "DXManager.h"
#include "IComponent.h"
#include "DXGameObject.h"
#include "CommonValues.h"

namespace MyDirectX 
{
	class Mover :public Component
	{
	public:
		Mover() {};
		~Mover() {};
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Update() override;
		void SetSpeed(float speed) { mMoveSpeed = speed; }
	private:
		DXInput* mDXInput = nullptr;
		//移動速度
		float mMoveSpeed = 0.02f;
		//ゲーム画面内にいるか
		bool IsInGameScreen(float screenX, float screenY);
	};
}



