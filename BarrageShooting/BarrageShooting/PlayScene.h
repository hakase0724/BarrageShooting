#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "BulletPool.h"
#include "BarrageManager.h"
#include "DXText.h"
#include "DXTexture.h"
#include "Player.h"
#include "ReactiveProperty.h"
#include "ScoreItemPool.h"
#include "ExplosionEffectPool.h"
#include "BombPool.h"
#include "PowerUpItemPool.h"

using namespace MyObservarPattern;

namespace MyDirectX
{
	class PlayScene :public Scene
	{
	public:
		PlayScene() {};
		~PlayScene() {};
		virtual void Init() override;
		virtual void SceneStart() override;
		virtual void SceneUpdate() override;
		virtual void SceneEnd() override;
		virtual bool IsSceneEnd() override;
		void AddScore(double score)
		{ 
			if(mIsCombo)
			{
				mComboScore += score;
				mComboCountFrame = 0;
			}
			else mIsCombo = true;
			*mScoreRP + score; 
		}
		void CreateBomb(DirectX::XMFLOAT3 pos);
		void CreatePowerUp(DirectX::XMFLOAT3 pos);
		DXGameObject* CreateExplosionEffect(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale = { 0.5f,0.5f,0.5f });
		DXGameObject* CreateScoreItem(DirectX::XMFLOAT3 pos);
		void BossDie() { mIsBossDie = true; }
	private:
		//���@����
		void CreatePlayer(LOAD_FROM_CSV_DATA data);
		//�{�X����
		void CreateBossEnemy(LOAD_FROM_CSV_DATA data);
		//�G���G����
		void CreateNormalEnemy(LOAD_FROM_CSV_DATA data);
		//�w�i����
		void CreateBackGround();
		//CSV�f�[�^���琶������
		void CreateFromCSVData();
		//UI�v�f����
		void CreateUIItem();
		//�I�u�W�F�N�g�v�[���쐬
		void CreateBulletPool();
		void CreateScoreItemPool();
		void CreateExplosionEffectPool();
		void CreateBombPool();
		void CreatePowerUpItemPool();
		//�R���{����
		void ComboAction();
		//�w�i�Ǘ��N���X�@2�̉摜���z�ړ�������
		std::unique_ptr<BackGround> mBackGround;
		//�I�u�W�F�N�g�v�[��
		std::unique_ptr<BulletPool> mBulletPool;
		std::unique_ptr<ScoreItemPool> mScoreItemPool;
		std::unique_ptr<ExplosionEffectPool> mExplosionEffectPool;
		std::unique_ptr<BombPool> mBombPool;
		std::unique_ptr<PowerUpItemPool> mPowerUpItemPool;
		//�e���Ǘ��N���X
		std::unique_ptr<BarrageManager> mBarrageManager;
		//�t���[���v���p
		int mFrameCount = 0;
		//FPS�\���X�V�t���[����
		const int FPS_CHEACK_FRAME_COUNT = 60;
		//FPS�\���e�L�X�g
		DXText* mFPSText = nullptr;
		//HP�\���e�L�X�g
		DXText* mHPText = nullptr;
		//Score�\���e�L�X�g
		DXText* mScoreText = nullptr;
		//�{���\���e�L�X�g
		DXText* mBombText = nullptr;
		//�V�[���J�n���ɃA�N�e�B�u�ɂ���I�u�W�F�N�g
		std::vector<DXGameObject*> mAwakeObject;
		//���@
		DXGameObject* mPlayer = nullptr;
		Player* mPlayerCom = nullptr;
		//�{�X�G
		DXGameObject* mEnemy = nullptr;
		//�E�F�[�u���Ǘ�����z��
		std::vector<std::vector<DXGameObject*>> mEnemyWaveList;
		//�ŏI�E�F�[�u��
		bool mIsLastWave = false;
		//���̃E�F�[�u��
		int mWaveCount = 0;
		//�G�C���A�X�e���v���[�g���g���Ė��O��Z�k
		using Property = ReactiveProperty<double>;
		//HP�\���Ɏg���ϐ�
		std::unique_ptr<Property, Deleter> mHpRP;
		//FPS�\���Ɏg���ϐ�
		std::unique_ptr<Property, Deleter> mFPSRP;
		//Score�\���Ɏg���ϐ�
		std::unique_ptr<Property, Deleter> mScoreRP;
		//Bomb�\���Ɏg���ϐ�
		std::unique_ptr<ReactiveProperty<int>, Deleter> mBombRP;
		//�R���{���̃X�R�A
		double mComboScore = 0.0;
		//�R���{����
		bool mIsCombo = false;
		//�R���{���̌o�߃t���[����
		int mComboCountFrame = 0;
		bool mIsBossDie = false;
		//�R���{�p���t���[����
		const int COMBO_LIMIT_FRAME = 30;
		//�\�ߍ��e�̐�
		const int PRE_CREATE_BULLET_NUM = 1000;
	};
}


