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
		//���ˑҋ@����
		bool IsCoolTime(int time) { return time % mCoolTime == 0; }
		//�e�̋O�����v�Z����
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
		//�e���̖��O������enum
		BarrageName mName;
		//�e�𔭎˂���t���[���Ԋu
		int mCoolTime;
		//�e�𔭎˂���ۂɓ����ɑłe��
		int mBulletNum;
		//�e��
		float mBulletSpeed;
		//�e�̌����o���p�x
		float mAngle;
		//�x���@�\�L�̊p�x���ʓx�@�\�L�̊p�x�ɕϊ�����
		float ToRadian(float angle) { return angle * (float)(acos(-1.0f) / 180.0f); }
		//�n���ꂽ�x�N�g���𐳋K������
		void Normalization(float &x, float &y);
		//�Ă΂ꂽ��
		int mCount;
		//�e��
		int mShotNum;
		//�e�������o�����p�x
		float mAllAngle;
		//�e��������
		int mShotCount;
		COLOR mColor;
		std::wstring mBulletTexturePath;
		BULLET_SETTING_DATA mBulletData;
	};
}


