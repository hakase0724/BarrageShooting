#pragma once
#include "BarrageBase.h"

namespace MyDirectX
{
	class SixWayRazerBarrage :public BarrageBase
	{
	public:
		SixWayRazerBarrage();
		~SixWayRazerBarrage() {};
		virtual void CreateBarrage(BulletPool* pool, DXGameObject* player, DXGameObject* enemy, float scaleRate = 1.0f) override;
	private:
		float mCosAngle;
		float mSinAngle;
	};
}


