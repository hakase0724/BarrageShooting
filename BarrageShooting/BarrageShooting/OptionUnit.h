#pragma once
#include "IComponent.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "Player.h"

namespace MyDirectX
{
	//class Player {};

	class OptionUnit :public Component
	{
	public:
		OptionUnit() {};
		~OptionUnit() {};
		//èâä˙âªèàóù
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		virtual void Update() override;
		void SetPlayer(DXGameObject* player);
		void SetBulletPool(BulletPool* bulletPool) { mBulletPool = bulletPool; };
		void SetXOffset(float xoffset) { mXOffset = xoffset; };
		void SetYOffset(float yoffset) { mYOffset = yoffset; };
		void SetXvectol(float vectol) { mXVectol = vectol; }
	private:
		DXGameObject* mPlayerGameObject = nullptr;
		BulletPool* mBulletPool = nullptr;
		Player* mPlayerCom = nullptr;
		TRANSFORM* mTransform = nullptr;
		TRANSFORM* mPlayerTransform = nullptr;
		float mXOffset = 0.0f;
		float mYOffset = 0.0f;
		float mXVectol = 0.0f;
	};
}


