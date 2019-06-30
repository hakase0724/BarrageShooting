#pragma once
#include "BarrageBase.h"

namespace MyDirectX
{
	class AllDirectionRotation :public BarrageBase
	{
	public:
		AllDirectionRotation();
		~AllDirectionRotation() {};
		virtual void CreateBarrage(BulletPool* pool, DXGameObject* player, DXGameObject* enemy, float scaleRate = 1.0f) override;
	private:
		float mCosAngle;
		float mSinAngle;
		float mShiftCosAngle;
		float mShiftSinAngle;
		float mCenterVectolX;
		float mCenterVectolY;
	};
}


