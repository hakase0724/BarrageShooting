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
		//�Z�b�^�[
		void SetBulletPool(BulletPool* pool) { mBulletPool = pool; }
		void SetPlayer(DXGameObject* player) { mPlayer = player; }
		void SetBarrageManager(BarrageManager* manager) { mBarrageManager = manager; }
		void SetDropItemType(DropItemType type) { mDropItemType = type; }
		//�̗͂̃C���^�[�t�F�[�X
		virtual void SetHP(double hp) override { mDefaultHitPoint = hp; }
		virtual double GetMaxHP() override { return mDefaultHitPoint; }
		virtual double GetHP() override { return mHitPoint; }
		virtual void Damage(double damage) override;
	protected:
		PlayScene* mPlayScene = nullptr;
		//�e�̃I�u�W�F�N�g�v�[��
		BulletPool* mBulletPool = nullptr;
		//���t���[���o�߂�����
		int mWaitCount = 0;
		//�e�𔭎˂���t���[���Ԋu
		int mCoolCount = 60;
		//�̗�
		double mHitPoint = 1.0;
		//�����̗�
		double mDefaultHitPoint = 1.0;
		//�v���C���[
		DXGameObject* mPlayer = nullptr;
		//���Z�b�g����Ă���e��
		BarrageBase* mBarrage = nullptr;
		//�e���Ǘ��N���X
		BarrageManager* mBarrageManager = nullptr;
		//�e�����ʂɎg�p
		BarrageName mBarrageName;
		//���ː�
		int mShotCount = 0;
		//�e���؂�ւ�
		virtual void ChangeBarrageName() {};
		//�e���I������
		virtual bool IsBarrageEnd();
		double mScore = 0.0;
		DropItemType mDropItemType = PowerUp;
	};
}


