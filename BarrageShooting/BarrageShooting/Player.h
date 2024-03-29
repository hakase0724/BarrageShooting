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
		//初期化処理
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		void SetBulletPool(BulletPool* pool) { mBulletPool = pool; }
		//更新処理
		virtual void Update() override;
		virtual void OnCollisionEnter2D(Collider2D* col) override;
		virtual void SetHP(double hp) { mDefaultHitPoint = hp; }
		virtual double GetMaxHP() override { return mDefaultHitPoint; }
		virtual double GetHP() override { return mHitPoint; }
		virtual void Damage(double damage) override;
		bool IsShot() const { return mIsShot; };
		bool IsLongPush() const { return mIsLongPush; };
		int GetBombNum() const { return mBombNum; }
		//ボムを追加
		void AddBomb() 
		{
			mBombNum++;
			if (mBombNum >= mMaxBombNum) mBombNum = mMaxBombNum;
		}
		//パワーアップ
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
		//今所持しているボム数
		int mBombNum = 0;
		//保有可能なボムの最大値　首領蜂シリーズは3回ボムを使うたびに最大数が上昇し最大7個まで増加する
		int mMaxBombNum = 3;
		//ボム使用回数
		int mUsedBombNum = 0;
		//発射可能かどうか
		bool CanShot();
		//何フレーム経過したか
		int mWaitCount = 0;
		//弾を発射するフレーム感覚
		int mCoolCount = 7;
		const int BULLET_COOL_COUNT = 3;
		const int RAZER_COOL_COUNT = 1;
		//体力
		double mHitPoint = 0.0;
		double mDefaultHitPoint = 1.0;
		//パワー
		int mPower = 1;
		//最大パワー
		int mMaxPower = 4;
		//長押し判定フラグ
		bool mIsLongPush = false;
		//発射しているか
		bool mIsShot = false;
		int mPushFrame = 0;
		int mJudgeLongPushFrame = 10;
		float mRazerWidth = 0.0f;
		float mAddValue = 0.01f;
		bool mIsInvincible = false;
	};
}
 

