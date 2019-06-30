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
		//�ŏ��̃x�N�g�������炷
		float mShiftCosAngle;
		float mShiftSinAngle;
	};
}


