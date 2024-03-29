#pragma once
#include "IComponent.h"
#include "DXManager.h"
#include "Bullet.h"
#include "Colliders.h"
#include "DXGameObject.h"
#include "AllDirectionSync.h"
#include "BarrageBase.h"
#include "BarrageManager.h"
#include "BulletPool.h"
#include "PlayScene.h"

namespace MyDirectX
{
	class EnemyBase :public Component,public IHP
	{
	public:
		EnemyBase() {};
		virtual ~EnemyBase() {};
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override {};
		virtual void Update() override {};
		virtual void OnCollisionEnter2D(Collider2D* col) override;
		virtual void OnDisable() override;
		//セッター
		void SetBulletPool(BulletPool* pool) { mBulletPool = pool; }
		void SetPlayer(DXGameObject* player) { mPlayer = player; }
		void SetBarrageManager(BarrageManager* manager) { mBarrageManager = manager; }
		void SetDropItemType(DropItemType type) { mDropItemType = type; }
		//体力のインターフェース
		virtual void SetHP(double hp) override { mDefaultHitPoint = hp; }
		virtual double GetMaxHP() override { return mDefaultHitPoint; }
		virtual double GetHP() override { return mHitPoint; }
		virtual void Damage(double damage) override;
	protected:
		PlayScene* mPlayScene = nullptr;
		//弾のオブジェクトプール
		BulletPool* mBulletPool = nullptr;
		//何フレーム経過したか
		int mWaitCount = 0;
		//弾を発射するフレーム間隔
		int mCoolCount = 60;
		//体力
		double mHitPoint = 1.0;
		//初期体力
		double mDefaultHitPoint = 1.0;
		//プレイヤー
		DXGameObject* mPlayer = nullptr;
		//今セットされている弾幕
		BarrageBase* mBarrage = nullptr;
		//弾幕管理クラス
		BarrageManager* mBarrageManager = nullptr;
		//弾幕識別に使用
		BarrageName mBarrageName;
		//発射数
		int mShotCount = 0;
		//弾幕切り替え
		virtual void ChangeBarrageName() {};
		//弾幕終了判定
		virtual bool IsBarrageEnd();
		double mScore = 0.0;
		DropItemType mDropItemType = PowerUp;
	};
}


