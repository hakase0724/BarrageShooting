#include "stdafx.h"
#include "BarrageManager.h"
#include "AllDirectionSync.h"
#include "TargetPlayerBarrage.h"
#include "SixWayRazerBarrage.h"
#include "FiveWaySpreadBarrage.h"
#include "AllDirectionRotation.h"

using namespace MyDirectX;
BarrageManager::BarrageManager()
{
	//SetBarrage<AllDirectionSync>();
	SetBarrage<TargetPlayerBarrage>();
	SetBarrage<SixWayRazerBarrage>();
	SetBarrage<FiveWaySpreadBarrage>();
	SetBarrage<AllDirectionRotation>();
}

BarrageBase * BarrageManager::GetBarrage(BarrageName name)
{
	BarrageBase* pReturn = nullptr;
	for(auto &barrage : mBarrageList)
	{
		if(barrage->GetName() == name)
		{
			pReturn = barrage.get();
			break;
		}
	}
	return pReturn;
}

std::vector<BarrageBase*> BarrageManager::GetBarrages()
{
	std::vector<BarrageBase*> barrages;
	for(auto &barrage:mBarrageList)
	{
		barrages.push_back(barrage.get());
	}
	return barrages;
}
