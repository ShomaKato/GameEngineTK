///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/05/15
//*
//*	@���O		FollowCamera.h
//*
//*	@����		���� ���n
//*
//* @����		���@�ɒǏ]����J�����N���X
//*
///-------------------------------------------------------------------

// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include "Camera.h"
#include <Keyboard.h>
#include "Player.h"

class FollowCamera :public Camera
{
public:
	// �ÓI�ȕϐ��̒�`
	//static const float FollowCamera::CAMERA_DISTANCE;	/* FollowCamera�̃R���A�Ƃ����w��� */

	FollowCamera(int width, int height);
	void Update() override;				/* ���z�֐��ɂ̂ݎg����B�������͂������̏�����ŁA�Ƃ����Ӗ��� */

	// ���_���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos);

	// �Ǐ]�Ώۂ̊p�x���Z�b�g
	void SetTargetAngle(const float& targetAngle);


	// �L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);

	// �v���C�����Z�b�g		/* �v���C��������Ȃ王�_�A�Ǐ]�Ώۂ̊p�x��Update�ōs�� */
	void SetPlayer(Player* player) { m_player = player; }


protected:		/* �����炭������p�����邱�Ƃ͂Ȃ��̂ŁAprivate�ł��܂������A�Ƃ̂��� */
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;
	
	// �L�[�{�[�h�i�l����������Ă���Q�Ɨp�j
	DirectX::Keyboard* m_keyboard;
	// TPF��FPS�̃g���K�[�����p�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	// �g���K�[
	bool m_isFPS;

	// �v���C��
	Player* m_player;	/* ����Game�ɂ���v���C�����������߂̕ϐ� */
};

