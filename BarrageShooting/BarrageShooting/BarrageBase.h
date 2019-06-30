#pragma once
#include "MyEnums.h"
#include <math.h>
#include <tuple>
#include "MyStructs.h"
namespace MyDirectX
{
	class BulletPool;
	class DXGameObject;
	class BarrageBase
	{
	public:
		BarrageBase() {};
		~BarrageBase() {};
		BarrageName GetName() const { return mName; }
		void Init() { mCount = 0; }
		//発射待機中か
		bool IsCoolTime(int time) { return time % mCoolTime == 0; }
		//弾の軌道を計算する
		virtual void CreateBarrage(BulletPool* pool, DXGameObject* player, DXGameObject* enemy,float scaleRate = 1.0f) = 0;
		int GetShotNum() const { return mShotNum; }
		void SetColor(COLOR color) 
		{
			mColor = color; 
			mBulletData.color = mColor;
		}
		void SetBulletTexture(const wchar_t* path) { mBulletTexturePath = path; }
		void SetBulletTexture(std::wstring path) { mBulletTexturePath = path; }
		bool IsBarrageEnd() 
		{
			if(mShotCount >= mShotNum)
			{
				mShotCount = 0;
				return true;
			}
			else return false;
		}
	protected:
		//弾幕の名前を示すenum
		BarrageName mName;
		//弾を発射するフレーム間隔
		int mCoolTime;
		//弾を発射する際に同時に打つ弾数
		int mBulletNum;
		//弾速
		float mBulletSpeed;
		//弾の撃ち出す角度
		float mAngle;
		//度数法表記の角度を弧度法表記の角度に変換する
		float ToRadian(float angle) { return angle * (float)(acos(-1.0f) / 180.0f); }
		//渡されたベクトルを正規化する
		void Normalization(float &x, float &y);
		//呼ばれた回数
		int mCount;
		//弾数
		int mShotNum;
		//弾を撃ち出す総角度
		float mAllAngle;
		//弾撃った数
		int mShotCount;
		COLOR mColor;
		std::wstring mBulletTexturePath;
		BULLET_SETTING_DATA mBulletData;
	};
}


