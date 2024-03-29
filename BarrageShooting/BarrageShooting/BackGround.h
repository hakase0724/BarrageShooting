#pragma once
#include "DXGameObject.h"

namespace MyDirectX
{
	class BackGround 
	{
	public:
		BackGround() {};
		~BackGround() {};
		void SetBackGrounds(DXGameObject* back1, DXGameObject* back2) 
		{
			mBackFirst = back1;
			mBackSecond = back2;
		}
		void ClearBackGrounds()
		{
			mBackFirst = nullptr;
			mBackSecond = nullptr;
		}
		void UpdateBackGrounds();
		void ChangeBackGroundEnable(bool enable)
		{
			mBackFirst->SetEnable(enable);
			mBackSecond->SetEnable(enable);
		}
	private:
		DXGameObject* mBackFirst = nullptr;
		DXGameObject* mBackSecond = nullptr;
		//背景の移動速度
		float mMoveSpeed = 0.01f;
		//背景が上に戻るY座標　この値以下になったら上に戻る
		const float MOVE_UP_Y_POSITION = -5.0f;
		//背景が上に戻る際の移動量
		const float MOVE_UP_VALUE = 10.0f;
	};
}


