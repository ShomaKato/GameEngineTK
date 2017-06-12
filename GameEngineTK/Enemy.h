///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/05
//*
//*	@���O		Enemy.h
//*
//* @����		�G�l�~�[�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------
// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�̃C���N���[�h
#include "Obj3d.h"

class Enemy
{
	// �p�[�c�����i�[����vector�^�R���e�i
	std::vector<Obj3d> enemy;

	// �p�[�c		/* �e�ł���΂���قǐ�� */
	enum ROBOT_PARTS
	{
		ROBOT_PARTS_CRAWLER,
		ROBOT_PARTS_HIP,
		ROBOT_PARTS_RWING,
		ROBOT_PARTS_LWING,
		ROBOT_PARTS_BODY,
		ROBOT_PARTS_HEAD,

		ROBOT_PARTS_NUM
	};

public:
	Enemy();
	~Enemy();

	// �������֐�
	void InitializeEnemy();
	// �X�V�֐�
	void UpdateEnemy();
	// �`��֐�
	void RenderEnemy();

	// �ڒn���Ă邩�ۂ�
	bool m_isLanding = true;
	// sin�p����
	float m_sinAngle;


	// �^�C�}�[
	int m_timer;
	// �p�x�̕ύX
	float m_DistAngle;
};