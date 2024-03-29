#pragma once
#include "BarrageBase.h"
namespace MyDirectX
{
	class FiveWaySpreadBarrage :public BarrageBase
	{
	public:
		FiveWaySpreadBarrage();
		~FiveWaySpreadBarrage() {};
		virtual void CreateBarrage(BulletPool* pool, DXGameObject* player, DXGameObject* enemy, float scaleRate = 1.0f) override;
	private:
		float mCosAngle;
		float mSinAngle;
		//最初のベクトルをずらす
		float mShiftCosAngle;
		float mShiftSinAngle;
	};
}


