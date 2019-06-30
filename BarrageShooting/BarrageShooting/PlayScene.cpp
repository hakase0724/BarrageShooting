#include "stdafx.h"
#include "PlayScene.h"
#include <DirectXMath.h>
#include "Mover.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "HPGauge.h"
#include "DXAnimation.h"
#include <sstream>
#include "Bomb.h"
#include "PowerUpItem.h"
#include "OptionUnit.h"
#include "ItemMover.h"
#include "ScoreItem.h"


using namespace MyDirectX;

void PlayScene::Init()
{
	//�e���Ǘ��N���X
	mBarrageManager = std::make_unique<BarrageManager>();

	CreateBulletPool();
	CreateScoreItemPool();
	CreateExplosionEffectPool();
	CreateBombPool();
	CreatePowerUpItemPool();
	CreateUIItem();
	CreateFromCSVData();
	CreateBackGround();

	//�e��J�E���g��������
	mWaveCount = 0;
	mFrameCount = FPS_CHEACK_FRAME_COUNT;

	//���w���������X�g�ɒǉ�
	for(auto game:mEnemyWaveList[0])
	{
		mAwakeObject.push_back(game);
	}

	//�S�ẴI�u�W�F�N�g�̏����ʒu��ݒ肷��
	for(auto &game: mGameObjectsList)
	{
		game->SetDefaultTransform();
	}
}

void PlayScene::SceneStart()
{
	//�E�F�[�u��񃊃Z�b�g
	mWaveCount = 0;
	mIsLastWave = false;
	mComboScore = 0.0;
	mComboCountFrame = 0;
	mIsCombo = false;
	*mScoreRP = 0;
	mIsBossDie = false;
	
	//�S�ẴI�u�W�F�N�g�̏�����
	for(auto &game:mGameObjectsList)
	{
		game->InitializeComponent();
	}
	//�ŏ����炢��I�u�W�F�N�g�̃A�N�e�B�u��
	for(auto game:mAwakeObject)
	{
		game->SetEnable(true);
	}
	//�ȍĐ�
	mDXRescourceManager->GetBGMDXSound()->Play();
}

void PlayScene::SceneUpdate()
{
	mFrameCount++;
	*mHpRP = mPlayerCom->GetHP();
	*mBombRP = mPlayerCom->GetBombNum();
	ComboAction();
	//FPS���v�Z���o�͂���
	//���t���[���o���ƕω�������������̂ň��Ԋu�ōX�V
	if (mFrameCount % FPS_CHEACK_FRAME_COUNT == 0)
	{
		*mFPSRP = mDXRescourceManager->GetFPS();
	}
	//�w�i�𓮂���
	mBackGround->UpdateBackGrounds();

	//���̃E�F�[�u�֍s����
	bool isNext = true;
	//���݃E�F�[�u�̓G����̂ł��������Ă����玟�E�F�[�u�֍s���Ȃ�
	for(auto game:mEnemyWaveList[mWaveCount])
	{
		if (game->GetEnable()) isNext = false;
	}

	if(isNext)
	{
		mWaveCount++;
		//�ŏI�E�F�[�u��
		if (mWaveCount == mEnemyWaveList.size() - 1) mIsLastWave = true;
		//�������E�F�[�u�����p�ӂ��Ă��鐔�𒴂����珈�����Ȃ�
		if (mWaveCount >= (int)mEnemyWaveList.size()) return;
		//���E�F�[�u�̓G���A�N�e�B�u��
		for (auto game : mEnemyWaveList[mWaveCount])
		{
			game->SetEnable(true);
		}
	}
}

void PlayScene::SceneEnd()
{
	mDXRescourceManager->SetScore((float)mScoreRP->GetValue());
	//�S�ẴI�u�W�F�N�g�̃A�N�e�B�u��؂�
	for (auto &game : mGameObjectsList)
	{
		game->SetEnable(false);
	}
	//�A�C�e���̃A�N�e�B�u��؂�
	for(auto &game :mGameObjectsList)
	{
		if(game->GetTag() == StaticInstantiateItem)
		{
			game->SetEnable(false);
		}
	}
	//���I�����A�C�e��������΍폜
	for (auto itr = mGameObjectsList.begin(); itr != mGameObjectsList.end();)
	{
		if (itr->get()->GetTag() == DynamicInstantiateItem)
		{
			itr->reset();
			itr = mGameObjectsList.erase(itr);
			continue;
		}
		else ++itr;
	}
	//�Ȃ��~�߂�
	mDXRescourceManager->GetBGMDXSound()->Stop();
}

bool PlayScene::IsSceneEnd()
{
	//���@�����񂾂�
	if (!mPlayer->GetEnable()) 
	{
		mDXRescourceManager->GetSEDXSound()->Stop();
		return true;
	}
	//�{�X�����񂾂�
	if(mIsBossDie)
	{
		mDXRescourceManager->GetSEDXSound()->Stop();
		return true;
	}
	return false;
}

void PlayScene::CreateBomb(DirectX::XMFLOAT3 pos)
{
	mBombPool->GetBomb(pos, DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
}

void PlayScene::CreatePowerUp(DirectX::XMFLOAT3 pos)
{
	mPowerUpItemPool->GetPowerUpItem(pos, DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
}

DXGameObject* PlayScene::CreateExplosionEffect(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	auto exSound = mDXRescourceManager->GetExplosionEffectSound();
	exSound->ResetSound();
	exSound->Play(false);
	return mExplosionEffectPool->GetExplosionEffect(pos,scale);
}

DXGameObject* PlayScene::CreateScoreItem(DirectX::XMFLOAT3 pos)
{
	return mScoreItemPool->GetScoreItem(pos);
}

void PlayScene::CreatePlayer(LOAD_FROM_CSV_DATA data)
{
	//���@
	mPlayer = Instantiate();
	mPlayer->SetName(data.Name);
	auto transform = mPlayer->GetTransform();
	transform->Position = data.StartPos;
	transform->Scale.x /= 3.0f;
	transform->Scale.y /= 3.0f;
	transform->Scale.z /= 3.0f;
	auto texture = mPlayer->AddComponent<DXTexture>();
	auto anim = mPlayer->AddComponent<DXAnimation>();
	anim->SetAnimationTexture(texture);
	anim->SetAnimationFile(PLAYER1_PATH);
	anim->SetAnimationFile(PLAYER2_PATH);
	mPlayer->SetTag(data.Tag);
	mPlayer->AddComponent<Mover>();
	mPlayerCom = mPlayer->AddComponent<Player>();
	mPlayerCom->SetBulletPool(mBulletPool.get());
	mPlayerCom->SetHP(data.HP);
	auto playerCol = mPlayer->AddComponent<SquareCollider2D>();
	//���@�̃R���C�_�[��30����1
	playerCol->SetOneSide(playerCol->GetOneSide() / 100.0f);
	mAwakeObject.push_back(mPlayer);

	auto rightOptionUnit = Instantiate();
	auto rightOptionTransform = rightOptionUnit->GetTransform();
	rightOptionTransform->Scale.x /= 5.0f;
	rightOptionTransform->Scale.y /= 5.0f;
	rightOptionTransform->Scale.z /= 5.0f;
	auto rightOptionCom = rightOptionUnit->AddComponent<OptionUnit>();
	rightOptionCom->SetPlayer(mPlayer);
	rightOptionCom->SetXOffset(0.3f);
	rightOptionCom->SetYOffset(0.3f);
	rightOptionCom->SetBulletPool(mBulletPool.get());
	rightOptionCom->SetXvectol(0.01f);
	auto rightOptionTex = rightOptionUnit->AddComponent<DXTexture>();
	rightOptionTex->SetTexture(SQUARE_PATH);
	rightOptionTex->SetDefaultColor(0.0f, 0.0f, 1.0f, 1.0f);
	mAwakeObject.push_back(rightOptionUnit);

	auto leftOptionUnit = Instantiate();
	auto leftOptionTransform = leftOptionUnit->GetTransform();
	leftOptionTransform->Scale.x /= 5.0f;
	leftOptionTransform->Scale.y /= 5.0f;
	leftOptionTransform->Scale.z /= 5.0f;
	auto leftOptionCom =leftOptionUnit->AddComponent<OptionUnit>();
	leftOptionCom->SetPlayer(mPlayer);
	leftOptionCom->SetXOffset(-0.3f);
	leftOptionCom->SetYOffset(0.3f);
	leftOptionCom->SetBulletPool(mBulletPool.get());
	leftOptionCom->SetXvectol(-0.01f);
	auto leftOptionTex = leftOptionUnit->AddComponent<DXTexture>();
	leftOptionTex->SetTexture(SQUARE_PATH);
	leftOptionTex->SetDefaultColor(0.0f, 0.0f, 1.0f, 1.0f);
	mAwakeObject.push_back(leftOptionUnit);
}

void PlayScene::CreateBossEnemy(LOAD_FROM_CSV_DATA data)
{
	//�{�X�G
	mEnemy = Instantiate();
	mEnemy->SetName(data.Name);
	mEnemy->SetTag(data.Tag);
	auto enemyPos = mEnemy->GetTransform();
	enemyPos->Position = data.StartPos;
	//�摜�𔽓]
	enemyPos->Scale = DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f);
	auto enemyTex = mEnemy->AddComponent<DXTexture>();
	auto enemyAnim = mEnemy->AddComponent<DXAnimation>();
	enemyAnim->SetAnimationTexture(enemyTex);
	enemyAnim->SetAnimationFile(BOSS1_PATH);
	enemyAnim->SetAnimationFile(BOSS2_PATH);
	enemyAnim->SetAnimationFile(BOSS3_PATH);
	enemyAnim->SetTextureChangeCount(3);
	auto enemy = mEnemy->AddComponent<BossEnemy>();
	enemy->SetBulletPool(mBulletPool.get());
	enemy->SetPlayer(mPlayer);
	enemy->SetBarrageManager(mBarrageManager.get());
	enemy->SetHP(data.HP);
	auto battleStartPos = data.StartPos;
	//�����ʒu����1�i�񂾂�e���W�J���J�n����悤�ɂ���
	battleStartPos.y -= 1;
	enemy->SetBattleStartPos(battleStartPos);
	auto enemyCol = mEnemy->AddComponent<SquareCollider2D>();
	enemyCol->SetOneSide(enemyCol->GetOneSide());
	enemy->SetCollider(enemyCol);

	//�̗̓Q�[�W
	auto gauge = Instantiate();
	auto gaugeTransform = gauge->GetTransform();
	//�Q�[�W�̈ʒu�ƒ���
	gaugeTransform->Position.y = 1.3f;
	gaugeTransform->Position.z = -0.1f;
	gaugeTransform->Scale.y = 0.1f;
	gaugeTransform->Scale.x = 3.0f;
	auto renderer = gauge->AddComponent<TextureRenderer>();
	auto gaugeTex = gauge->AddComponent<DXTexture>();
	auto gaugeCom = gauge->AddComponent<HPGauge>();
	gaugeCom->SetHPViewObject(enemy);
	//�Q�[�W�̐F
	renderer->SetDefaultColor(1.0f, 0.8f, 0.0f, 0.5f);
	gaugeTex->SetTexture(SQUARE_PATH);
	

	mEnemyWaveList[data.Wave - 1].push_back(mEnemy);
	mEnemyWaveList[data.Wave - 1].push_back(gauge);
}

void PlayScene::CreateNormalEnemy(LOAD_FROM_CSV_DATA data)
{
	auto normalEnemy = Instantiate();
	normalEnemy->SetName(data.Name);
	normalEnemy->SetTag(data.Tag);
	auto normaEenemyCom = normalEnemy->AddComponent<NormalEnemy>();
	normaEenemyCom->SetHP(data.HP);
	normaEenemyCom->SetBulletPool(mBulletPool.get());
	normaEenemyCom->SetPlayer(mPlayer);
	normaEenemyCom->SetBarrageManager(mBarrageManager.get());
	auto normalEnemyTex = normalEnemy->AddComponent<DXTexture>();
	COLOR color;
	switch (data.Category)
	{
	case CategoryRedEnemy:
		normalEnemyTex->SetTexture(RED_ENEMY_PATH);
		normaEenemyCom->SetDropItemType(DropItemType::PowerUp);
		normaEenemyCom->SetBulletTexture(WHITE_TO_RED_BULLET_PATH);
		break;
	case CategoryBlueEnemy:
		normalEnemyTex->SetTexture(BLUE_ENEMY_PATH);
		normaEenemyCom->SetDropItemType(DropItemType::BombItem);
		normaEenemyCom->SetBulletTexture(BLUE_TO_LIGHTBLUE_BULLET_PATH);
		break;
	case CategoryGreenEnemy:
		normalEnemyTex->SetTexture(GREEN_ENEMY_PATH);
		normaEenemyCom->SetDropItemType(DropItemType::Score);
		normaEenemyCom->SetBulletTexture(WHITE_TO_PURPLE_BULLET_PATH);
		break;
	default:
		return;
		break;
	}
	normaEenemyCom->SetColor(color);
	auto normalEnemyTexPos = normalEnemy->GetTransform();
	normalEnemyTexPos->Position = data.StartPos;
	//�T�C�Y�𔼕���
	normalEnemyTexPos->Scale.x /= 2;
	normalEnemyTexPos->Scale.y /= 2;
	normalEnemyTexPos->Scale.z /= 2;
	
	auto normalEnemyCol = normalEnemy->AddComponent<SquareCollider2D>();
	//�G�̃R���C�_�[��2����1��
	normalEnemyCol->SetOneSide(normalEnemyCol->GetOneSide() / 2.0f);

	mEnemyWaveList[data.Wave - 1].push_back(normalEnemy);
}

void PlayScene::CreateBackGround()
{
	/*
	�w�i�𓮂����C���[�W

	�@�@�@�@�@�@ ��
		�w �i 2�@��
		   ���@�@��
		�w �i 1�@��
		   ���@�@��
		   ��������
	*/

	//�w�i�p�摜1
	auto back = Instantiate();
	auto texBack = back->AddComponent<DXTexture>();
	texBack->SetTexture(BACKGROUND_PATH);
	auto backTransform = back->GetTransform();
	backTransform->Position.z = 2.0f;
	backTransform->Scale.x = 5.0f;
	backTransform->Scale.y = 5.0f;
	mAwakeObject.push_back(back);

	//�w�i�p�摜2
	auto back2 = Instantiate();
	auto texBack2 = back2->AddComponent<DXTexture>();
	texBack2->SetTexture(BACKGROUND_PATH);
	auto backTransform2 = back2->GetTransform();
	backTransform2->Position.y = 5.0f;
	backTransform2->Position.z = 2.0f;
	backTransform2->Scale.x = 5.0f;
	backTransform2->Scale.y = 5.0f;
	mAwakeObject.push_back(back2);

	//�w�i�𓮂����N���X
	mBackGround = std::make_unique<BackGround>();
	mBackGround->SetBackGrounds(back, back2);

	/*

	���с@�@�@ ����
	 ���@�@�@�@ ��
	*** �w�@�i ***
	*** �w�@�i ***
	*** �w�@�i ***
	*** �w�@�i ***

	*/

	//�E�̍���
	auto black = Instantiate();
	auto blackBack = black->AddComponent<DXTexture>();
	blackBack->SetTexture(BLACK_TEXTURE_PATH);
	auto blackTransform = black->GetTransform();
	blackTransform->Position.z = -1.0f;
	blackTransform->Position.x = 1.63f;
	blackTransform->Scale.y = 3.0f;
	mAwakeObject.push_back(black);

	//���̍���
	auto black2 = Instantiate();
	auto blackBack2 = black2->AddComponent<DXTexture>();
	blackBack2->SetTexture(BLACK_TEXTURE_PATH);
	auto blackTransform2 = black2->GetTransform();
	blackTransform2->Position.z = -1.0f;
	blackTransform2->Position.x = -1.63f;
	blackTransform2->Scale.y = 3.0f;
	mAwakeObject.push_back(black2);
}

void PlayScene::CreateFromCSVData()
{
	//�f�[�^�ǂݍ���
	auto dataTable = mDXRescourceManager->LoadData(CSV_DATA_PATH);

	//���E�F�[�u�܂ł���̂�
	int maxWave = 0;
	for (auto data : dataTable)
	{
		if (data.Wave > maxWave) maxWave = data.Wave;
	}

	//�E�F�[�u���z���p�ӂ���
	for (int i = 0; i < maxWave; i++)
	{
		std::vector<DXGameObject*> enemyList;
		mEnemyWaveList.push_back(enemyList);
	}

	//�ǂݍ��񂾃f�[�^�ɏ]���Ď��@�ƓG�����
	for (auto data : dataTable)
	{
		switch (data.Category)
		{
		case CategoryPlayer:
			CreatePlayer(data);
			break;
		case CategoryBossEnemy:
			CreateBossEnemy(data);
			break;
		case CategoryRedEnemy:
			CreateNormalEnemy(data);
			break;
		case CategoryBlueEnemy:
			CreateNormalEnemy(data);
			break;
		case CategoryGreenEnemy:
			CreateNormalEnemy(data);
			break;
		default:
			break;
		}
	}
}

void PlayScene::CreateUIItem()
{
	//HP�\���e�L�X�g
	auto hpNotice = Instantiate();
	auto hpNoticeText = hpNotice->AddComponent<DXText>();
	auto hpNoticeTransform = hpNotice->GetTransform();
	//�ʒu�Ƒ傫��
	hpNoticeTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	hpNoticeTransform->Position = DirectX::XMFLOAT3(1.2f, 0.9f, -1.1f);
	hpNoticeText->UpdateText(L"HP");
	mAwakeObject.push_back(hpNotice);
	//HP�\���e�L�X�g
	auto hp = Instantiate();
	mHPText = hp->AddComponent<DXText>();
	auto hpTransform = hp->GetTransform();
	//�ʒu�Ƒ傫��
	hpTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	hpTransform->Position = DirectX::XMFLOAT3(1.2f, 0.8f, -1.1f);
	mAwakeObject.push_back(hp);

	//BOMB�\���e�L�X�g
	auto bombNotice = Instantiate();
	auto bombNoticeText = bombNotice->AddComponent<DXText>();
	auto bombNoticeTransform = bombNotice->GetTransform();
	//�ʒu�Ƒ傫��
	bombNoticeTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	bombNoticeTransform->Position = DirectX::XMFLOAT3(1.2f, 0.6f, -1.1f);
	bombNoticeText->UpdateText(L"BOMB");
	mAwakeObject.push_back(bombNotice);
	//BOMB�\���e�L�X�g
	auto bomb = Instantiate();
	mBombText = bomb->AddComponent<DXText>();
	auto bombTransform = bomb->GetTransform();
	//�ʒu�Ƒ傫��
	bombTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	bombTransform->Position = DirectX::XMFLOAT3(1.2f, 0.5f, -1.1f);
	mAwakeObject.push_back(bomb);

	//Score�\���e�L�X�g
	auto scoreNotice = Instantiate();
	auto scoreText = scoreNotice->AddComponent<DXText>();
	auto scoreNoticeTransform = scoreNotice->GetTransform();
	//�ʒu�Ƒ傫��
	scoreNoticeTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	scoreNoticeTransform->Position = DirectX::XMFLOAT3(-1.7f, 0.9f, -1.1f);
	scoreText->UpdateText(L"SCORE");
	mAwakeObject.push_back(scoreNotice);
	//Score�\���e�L�X�g
	auto score = Instantiate();
	mScoreText = score->AddComponent<DXText>();
	auto scoreTransform = score->GetTransform();
	//�ʒu�Ƒ傫��
	scoreTransform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	scoreTransform->Position = DirectX::XMFLOAT3(-1.7f, 0.8f, -1.1f);
	mAwakeObject.push_back(score);

	//FPS�\���e�L�X�g
	auto fps = Instantiate();
	mFPSText = fps->AddComponent<DXText>();
	auto transform = fps->GetTransform();
	//�ʒu�Ƒ傫��
	transform->Scale = DirectX::XMFLOAT3(0.07f, 0.07f, 1.0f);
	transform->Position = DirectX::XMFLOAT3(1.2f, -0.8f, -1.1f);
	mAwakeObject.push_back(fps);

	//HP�\���Ɏg���ϐ�
	mHpRP =
		std::unique_ptr<Property, Deleter>
		(new Property(), Deleter());

	//�ϐ��ɕω������������ɕ\�����鏈����o�^
	mHpRP->Subscribe
	(
	[&](double value)->void
	{
		std::wstringstream hp;
		hp.precision(6);
		hp << value;
		mHPText->UpdateText(hp.str().c_str());
		hp.clear();
	}
	);

	mBombRP =
		std::unique_ptr<ReactiveProperty<int>, Deleter>
		(new ReactiveProperty<int>(),Deleter());

	mBombRP->Subscribe
	(
	[&](int value)->void
	{
		std::wstringstream bomb;
		bomb << value;
		mBombText->UpdateText(bomb.str().c_str());
		bomb.clear();
	}
	);

	// Score�\���Ɏg���ϐ�
	mScoreRP =
		std::unique_ptr<Property, Deleter>
		(new Property(), Deleter());

	//�ϐ��ɕω������������ɕ\�����鏈����o�^
	mScoreRP->Subscribe
	(
	[&](double value)->void
	{
		std::wstringstream score;
		score.precision(6);
		score << value;
		mScoreText->UpdateText(score.str().c_str());
		score.clear();
	}
	);

	//FPS�\���Ɏg���ϐ�
	mFPSRP =
		std::unique_ptr<Property, Deleter>
		(new Property(), Deleter());

	//�ϐ��ɕω������������ɕ\�����鏈����o�^
	mFPSRP->Subscribe
	(
	[&](double value)->void
	{
		mFrameCount = 0;
		//�����o�ϐ��Ɏ��ƒl���Œ肳��Ă��܂��E�E�E
		std::wstringstream ws;
		//�L�������w��
		ws.precision(6);
		ws << value;
		mFPSText->UpdateText(ws.str().c_str());
		//���g���N���A����
		ws.clear();
	}
	);
}

void PlayScene::CreateBulletPool()
{
	//�e�̃I�u�W�F�N�g�v�[��
	mBulletPool = std::make_unique<BulletPool>();
	mBulletPool->SetScene(this);
	//�\�ߒe��p�ӂ��Ă���
	mBulletPool->CreatePreBullets(PRE_CREATE_BULLET_NUM);
}

void PlayScene::CreateScoreItemPool()
{
	mScoreItemPool = std::make_unique<ScoreItemPool>();
	mScoreItemPool->SetScene(this);
	mScoreItemPool->CreatePreScoreItem(500);
}

void PlayScene::CreateExplosionEffectPool()
{
	mExplosionEffectPool = std::make_unique<ExplosionEffectPool>();
	mExplosionEffectPool->SetScene(this);
	mExplosionEffectPool->CreatePreExplosionEffect(100);
}

void PlayScene::CreateBombPool()
{
	mBombPool = std::make_unique<BombPool>();
	mBombPool->SetScene(this);
	mBombPool->CreatePreBomb(10);
}

void PlayScene::CreatePowerUpItemPool()
{
	mPowerUpItemPool = std::make_unique<PowerUpItemPool>();
	mPowerUpItemPool->SetScene(this);
	mPowerUpItemPool->CreatePrePowerUpItem(10);
}

void PlayScene::ComboAction()
{
	//�R���{���łȂ���Ώ��������Ȃ�
	if (!mIsCombo) return;
	//�o�߃t���[���J�E���g
	mComboCountFrame++;
	//���t���[���ȏ�o�߂�����
	if(mComboCountFrame > COMBO_LIMIT_FRAME)
	{
		mIsCombo = false;
		*mScoreRP + mComboScore;
		mComboScore = 0;
		mComboCountFrame = 0;
	}
}

