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
#include "CollisionNode.h"

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


	// 6/19 �����蔻�胂�f���̓ǂݍ��݁E�e�ۗp�̓����蔻��
	SphereNode m_collisionNode;

public:
	Enemy();
	~Enemy();

	// �������֐�
	void InitializeEnemy();
	// �X�V�֐�
	void UpdateEnemy();
	// �s��X�V
	void Calc();
	// �`��֐�
	void RenderEnemy();


	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	// ��]���擾
	const DirectX::SimpleMath::Vector3& GetRot();
	// ���W��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ��]��ݒ�
	void SetRot(const DirectX::SimpleMath::Vector3& rot);

	/* SetTrans��SetRot�̈�����const����������G���[�������� */
	/* �����̓�͌p���֐��B�p������const�����ĂȂ��������߃G���[������ */

	// ���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();


	// �ڒn���Ă邩�ۂ�
	bool m_isLanding = true;
	// sin�p����
	float m_sinAngle;


	// �^�C�}�[
	int m_Timer;
	// �p�x�̕ύX
	float m_DistAngle;


	// 6/19
	// �����蔻��(�Ƃ肠�������^)���擾
	const SphereNode& GetCollisionNodeBullet() { return m_collisionNode; }
	// �����蔻��̕\����\���t���O
	bool isCollisionVisible;
};