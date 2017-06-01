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

class Player
{
	// �p�[�c�����i�[����vector�^�R���e�i
	std::vector<Obj3d> player;

	// �L�[�{�[�h�̏��
	DirectX::Keyboard* Keyboard;

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

public:
	Player();
	~Player();

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


	// �ڒn���Ă邩�ۂ�
	bool m_isLanding = true;
	// sin�p����
	float m_sinAngle;
};