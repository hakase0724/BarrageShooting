#pragma once
#include "DXManager.h"
#include "IComponent.h"
#include "DXGameObject.h"
#include "Colliders.h"
#include <string>
#include "Bullet.h"
#include "BulletPool.h"

namespace MyDirectX
{
	class Mover;

	class Player :public Component,public IHP
	{
	public:
		Player() {};
		~Player() {};
		//����������
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		void SetBulletPool(BulletPool* pool) { mBulletPool = pool; }
		//�X�V����
		virtual void Update() override;
		virtual void OnCollisionEnter2D(Collider2D* col) override;
		virtual void SetHP(double hp) { mDefaultHitPoint = hp; }
		virtual double GetMaxHP() override { return mDefaultHitPoint; }
		virtual double GetHP() override { return mHitPoint; }
		virtual void Damage(double damage) override;
		bool IsShot() const { return mIsShot; };
		bool IsLongPush() const { return mIsLongPush; };
		int GetBombNum() const { return mBombNum; }
		//�{����ǉ�
		void AddBomb() 
		{
			mBombNum++;
			if (mBombNum >= mMaxBombNum) mBombNum = mMaxBombNum;
		}
		//�p���[�A�b�v
		void PowerUp()
		{
			mPower++;
			if (mPower >= mMaxPower) mPower = mMaxPower;
		}
	private:
		void Bomb();
		void Shot();
		void ShotBullet();
		void ShotRazer();
		bool IsMyLongPush();
		DXInput* mDXInput = nullptr;
		BulletPool* mBulletPool = nullptr;
		Mover* mMover = nullptr;
		//���������Ă���{����
		int mBombNum = 0;
		//�ۗL�\�ȃ{���̍ő�l�@��̖I�V���[�Y��3��{�����g�����тɍő吔���㏸���ő�7�܂ő�������
		int mMaxBombNum = 3;
		//�{���g�p��
		int mUsedBombNum = 0;
		//���ˉ\���ǂ���
		bool CanShot();
		//���t���[���o�߂�����
		int mWaitCount = 0;
		//�e�𔭎˂���t���[�����o
		int mCoolCount = 7;
		const int BULLET_COOL_COUNT = 3;
		const int RAZER_COOL_COUNT = 1;
		//�̗�
		double mHitPoint = 0.0;
		double mDefaultHitPoint = 1.0;
		//�p���[
		int mPower = 1;
		//�ő�p���[
		int mMaxPower = 4;
		//����������t���O
		bool mIsLongPush = false;
		//���˂��Ă��邩
		bool mIsShot = false;
		int mPushFrame = 0;
		int mJudgeLongPushFrame = 10;
		float mRazerWidth = 0.0f;
		float mAddValue = 0.01f;
		bool mIsInvincible = false;
	};
}
 

