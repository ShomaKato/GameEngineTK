///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/01
//*
//*	@���O		Player.h
//*
//* @����		�v���C���[�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------
// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�̃C���N���[�h
#include <Keyboard.h>
#include "Obj3d.h"
#include "CollisionNode.h"

class Player
{
	// �p�[�c�����i�[����vector�^�R���e�i
	std::vector<Obj3d> player;

	// �L�[�{�[�h�̏��
	DirectX::Keyboard* Keyboard;
	// ��񕪂����������Ă����g���K�[�����p�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker keyboardTracker;

	// ���@�p�[�c
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
	SphereNode m_collisionNodeBullet;

public:
	Player();
	~Player();

	// �������֐�
	void InitializePlayer();
	// �X�V�֐�
	void UpdatePlayer();
	// �`��֐�
	void RenderPlayer();

	// �L�[�{�[�h����n���֐�
	void SetKeyboard(DirectX::Keyboard* key);
	// �Q�Ɠ_���󂯎��֐�
	const DirectX::SimpleMath::Vector3 GetPlayerTranslation();
	// ���_���󂯎��֐�
	float GetPlayerRotationY();
	// �e�ہi�C�ӂ̃p�[�c�j�𔭎˂���֐�
	void FireBullet();
	// �e�ۂ��đ��U�i�p�[�c�����Z�b�g�j����֐�
	void ResetBullet();


	// �ڒn���Ă邩�ۂ�
	bool m_isLanding = true;
	// sin�p����
	float m_sinAngle;


	// �e�𔭎˂������ۂ�
	bool isEmpty;
	// �e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletV;
	// �e�۔��˕b���̃J�E���g�ϐ�
	int cntFiring;


	// 6/19
	// �e�ۗp�����蔻����擾
	const SphereNode& GetCollisionNodeBullet() { return m_collisionNodeBullet; }
	// �����蔻��̕\����\���t���O
	bool isCollisionVisible;


	// 7/10�@�v���C���ɂ��S�g�����蔻���
	// �S�g�̓����蔻��
	SphereNode m_collisionNode;
	// �����蔻��(�Ƃ肠�������^)���擾
	const SphereNode& GetCollisionNode() { return m_collisionNode; }
};