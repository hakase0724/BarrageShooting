#include "stdafx.h"
#include "Player.h"
#include "Mover.h"
#include <math.h>
#include "DXAnimation.h"

using namespace MyDirectX;

void Player::Initialize(DXGameObject * gameObject)
{
	mGameObject = gameObject;
	mDXInput = mGameObject->GetDXInput();
	auto transform = mGameObject->GetTransform();
	mId = mGameObject->GetID();
	mGameObject->SetDefaultTransform(transform);
	mCoolCount = BULLET_COOL_COUNT;
	mWaitCount = mCoolCount;
	mHitPoint = 1.0;
}

void Player::Initialize()
{
	mGameObject->ResetTransform();
	mCoolCount = BULLET_COOL_COUNT;
	mWaitCount = mCoolCount;
	if (mIsInvincible) mIsInvincible = false;
	mHitPoint = mDefaultHitPoint;
	mBombNum = 0;
	mMaxBombNum = 3;
	mPower = 1;
	mMaxPower = 4;
	mIsLongPush = false;
	//�ړ��R���|�[�l���g�擾
	if (mMover == nullptr) mMover = mGameObject->GetComponent<Mover>();
	mRazerWidth = 0.6f;
}

void Player::Update()
{
#if _DEBUG
	//�G���^�[�L�[�Ŗ��G���[�h��
	if (mDXInput->GetKeyDown(DIK_NUMPADENTER))
	{
		mIsInvincible = true;
		mHitPoint = 100;
	}
#endif

	//���t���[���J�E���g���s��
	mWaitCount++;
	//�łĂ�t���[����
	mIsShot = false;

	//�{��
	if (mDXInput->GetKeyDown(DIK_X))
	{
		Bomb();
	}

	//�V���b�g�{�^����������Ă��邩
	if (mDXInput->GetKey(DIK_Z))
	{
		//���˔���
		mIsShot = CanShot();
		//����������
		mIsLongPush = IsMyLongPush();
		if(mIsLongPush)
		{
			mMover->SetSpeed(0.01f);
		}
		else
		{
			mMover->SetSpeed(0.02f);
		}
	}
	else
	{
		mMover->SetSpeed(0.02f);
		//������Ă��Ȃ��Ȃ璷��������Ă��Ȃ�
		mIsLongPush = false;
		//���˂��Ȃ������ꍇ�͔��ˉ\��Ԃɂ��Ă���
		//-1���Ă���͎̂��̔���ł�+1���Ĕ��肷�邩��
		mWaitCount = mCoolCount - 1;
		//SE�����Z�b�g���Ă���
		mGameObject->GetDXResourceManager()->GetSEDXSound()->Stop();
	}

	if (mIsShot) Shot();
}

void Player::OnCollisionEnter2D(Collider2D* col)
{
	auto game = col->GetGameObject();
	//�A�C�e���Ƃ̏Փ˂ł̓_���[�W���󂯂Ȃ�
	if (game->GetTag() == DynamicInstantiateItem) return;
	if (game->GetTag() == StaticInstantiateItem) return;
	Damage(1.0);
}

void Player::Damage(double damage)
{
	if(!mIsInvincible)
	{
		mHitPoint -= damage;
	}
	//�̗͂��Ȃ��Ȃ����玩�g�̃A�N�e�B�u��؂�
	if (mHitPoint <= 0)
	{
		mGameObject->SetEnable(false);
	}
}

void Player::Bomb()
{
	//�{���������Ă��Ȃ���Ή������Ȃ�
	if (mBombNum <= 0) return;
	//�V�[���ɓo�^����Ă���I�u�W�F�N�g���擾
	auto gameObjects = mGameObject->GetScene()->GetGameObjects();
	for (auto &game : *gameObjects)
	{
		//��A�N�e�B�u�Ȃ疳��
		if (!game->GetEnable()) continue;
		auto tag = game->GetTag();
		//���@�������͎��e�Ȃ疳��
		if (tag == PlayerTag) continue;
		if (tag == PlayerBullet) continue;
		//HP�������Ă���R���|�[�l���g���擾
		auto gameHaveHP = game->GetComponent<IHP>();
		//HP������Ȃ�_���[�W��^����
		if (gameHaveHP != nullptr)
		{
			gameHaveHP->Damage(10.0);
		}
		//�Ȃ���΃A�N�e�B�u��؂�
		else
		{
			game->SetEnable(false);
		}
	}
	mUsedBombNum++;
	//�g�p�񐔂��ő及�L���𒴂�����ő及�L�����X�V
	if (mUsedBombNum >= mMaxBombNum)
	{
		mUsedBombNum = 0;
		mMaxBombNum++;
	}
	mBombNum--;
}

void Player::Shot()
{

	//���������Ă��邩
	if (mIsLongPush)
	{
		ShotRazer();	
	}
	//���������Ă��Ȃ�
	else
	{
		ShotBullet();
	}
	
	mGameObject->GetDXResourceManager()->GetSEDXSound()->ResetSound();
	mGameObject->GetDXResourceManager()->GetSEDXSound()->Play();
}

void Player::ShotBullet()
{
	mCoolCount = BULLET_COOL_COUNT;
	//�e�f�[�^
	BULLET_SETTING_DATA data = BULLET_SETTING_DATA();
	data.transform = *mGameObject->GetTransform();
	data.tag = PlayerBullet;
	data.xVectol = 0.0f;
	data.yVectol = 0.05f;
	data.ScaleRatio(0.3f);
	// �e�摜
	data.texturePath = BLUE_TO_LIGHTBLUE_BULLET_PATH;
	//�e��ł�
	for (int i = 0; i < mPower; i++)
	{
		//�e���o��
		auto game = mBulletPool->GetBullet(data);
		auto gameTransform = game->GetTransform();
		//�e�e���m�̊Ԋu
		auto offset = gameTransform->Scale.x;
		//�e���m�̃I�t�Z�b�g�v�Z
		gameTransform->Position.x += offset * (i - (float)mPower / 3.0f);
	}
}

void Player::ShotRazer()
{
	mCoolCount = RAZER_COOL_COUNT;
	mRazerWidth += mAddValue;
	//�e�f�[�^
	BULLET_SETTING_DATA data;
	data.pTransform = mGameObject->GetTransform();
	data.transform = *mGameObject->GetTransform();
	data.transform.Position.y += 0.3f;
	data.tag = PlayerBullet;
	data.xVectol = 0.0f;
	data.yVectol = 0.05f;
	data.isXFixed = true;
	//�����𓮂����΃��[�U�[�ł��Ă�ۂ��Ȃ�Ȃ����ȁE�E�H
	if (mRazerWidth < 0.6f) mAddValue = 0.01f;
	if (mRazerWidth > 0.7f) mAddValue = -0.01f;
	data.scaleXRatio = mRazerWidth;
	// �e�摜
	data.texturePath = RAZER_PATH;
	//�e���o��
	mBulletPool->GetBullet(data);
}

bool Player::IsMyLongPush()
{
	//���ɒ���������ς݂Ȃ画�肵�Ȃ�
	if (mIsLongPush) return true;
	//�����n�߂̃t���[����
	if (mDXInput->GetKeyDown(DIK_Z))
	{
		//�J�E���g���Z�b�g
		mPushFrame = 0;
		return false;
	}
	else
	{
		//�J�E���g�A�b�v
		mPushFrame++;
		//����������t���[������蒷���ԉ����Ă���
		if(mPushFrame > mJudgeLongPushFrame)
		{
			//�J�E���g���Z�b�g
			mPushFrame = 0;
			return true;
		}
	}
	//������Ă��Ȃ�
	return false;
}

bool Player::CanShot()
{
	if (mWaitCount % mCoolCount == 0)
	{
		mWaitCount = 0;
		return true;
	}
	else
	{
		return false;
	}
}