#pragma once
namespace MyDirectX
{
	enum RenderingObjectType
	{
		Texture = 0,
		Text = 1,
	};
	//�e�Q�[���I�u�W�F�N�g�ɐݒ�ł���^�O
	enum Tag
	{
		//�v���C���[
		PlayerTag = 0,
		//�G
		EnemyTag = 1,
		//���e
		PlayerBullet = 2,
		//�G�e
		EnemyBullet = 3,
		//���I��������A�C�e���@�V�[���I�����ɂ���Δj�������
		DynamicInstantiateItem = 4,
		//�ÓI��������A�C�e���@�V�[���I�����ɔj������Ȃ�
		StaticInstantiateItem = 5,
	};

	enum Category
	{
		CategoryPlayer = 0,
		CategoryBossEnemy = 1,
		CategoryRedEnemy = 2,
		CategoryBlueEnemy = 3,
		CategoryGreenEnemy = 4,
	};

	//�Ăяo���Փˉ���������
	enum CollisionType
	{
		//�Փ˂��Ă��Ȃ�
		NoCollision = 0,
		//�Փ˂��n�߂�
		Enter = 1,
		//�Փ˂��Ă���
		Stay = 2,
		//�Փ˂��I�����
		Exit = 3
	};

	enum BarrageName
	{
		AllDirectionsSync = 0,
		TargetPlayer = 1,
		SixWayRazer = 2,
		FiveWaySpread = 3,
		AllDirectionsRotation = 4,
	};

	enum SceneState
	{
		Title = 0,
		Play = 1,
		Result = 2,
	};

	enum BossState
	{
		Entry = 0,
		Battle = 1,
		BossDie = 2
	};

	enum DropItemType
	{
		NoDrop = -1,
		Score = 0,
		PowerUp = 1,
		BombItem = 2,
	};
}