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

// �w�b�_�t�@�C���̃C���N���[�h
#include "pch.h"
#include "FollowCamera.h"

// ���O���
using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�ȉ]�X����ʂ�
//const float CAMERA_DISTANCE = 5.0;



////----------------------------------------------------------------------
////! @�֐����FFollowCamera
////!
////! @�����F�J�����̏���������
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)							/* �f�t�H���g�R���X�g���N�^���Ȃ����߂ɃR�����K�{�A�炵�� */
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;

	m_keyboard = nullptr;

	m_isFPS = false;

	m_player = nullptr;
}

void FollowCamera::Update()		/* ���@�̈ړ���������ɍs�� */
{

	//=======================�L�[�{�[�h����==========================//

	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardstate);

	// C�L�[���������Ƃ�FPS/TPS��؂�ւ�
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		// �t���O��؂�ւ�
		m_isFPS = !m_isFPS;
	}

	//=======================�v���C������==========================//
	if (m_player)
	{
		// �ڕW���W�̐ݒ�
		SetTargetPos(m_player->GetPlayerTranslation());
		SetTargetAngle(m_player->GetPlayerRotationY());
	}


	//=======================�J��������==========================//
	if (!m_isFPS)
	{
		// �g���K�[��false�Ȃ�TPS�J�����ݒ�
		// ���_,�Q�Ɠ_			/* ���_�́u�J�����ʒu�v�@�Q�Ɠ_�́u�J�����������ɑ�����ʒu�v */
		Vector3 eyepos, refpos;

		// ���@��2m����Q�Ɠ_��
		refpos = m_targetPos + Vector3(0, 2.0f, 0);
		// �Q�Ɠ_�Ǝ��_�̍����x�N�g��
		Vector3 cameraV(0, 0, 5.0f);

		// ���@�̌��ɉ�荞�ނ��߂̉�]�s��𐶐�
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);

		// �����x�N�g������]������
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// �J�������W���v�Z
		eyepos = refpos + cameraV;


		//=======================�S���R������=======================//

		// ���_�����݈ʒu�����Ԃ���
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.15f;
		/* �O�t���[���̃J�����ʒu��m_eyepos�i�܂�Update�|����O������j
		�@ ���_�i�ړI�n�j�ƑO�t���[�����_�i���ݒn�j�̍����~5�� */
		  // 1�t���[��������1�i�ނƂ�����A�������0.05�����i�ނ悤�ɋ������Ă�C���[�W�H

		  /* ���̏�Ԃł���̂ł��Ă��邪�A�Q�Ɠ_��������Ȃ��Ǝ��@����ɉ�ʒ����ɂ���킯�ł͂Ȃ��Ȃ� */


		  // �Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_refpos + (refpos - m_refpos) * 0.05f;

		/* ��ɓ��� */		// �����A�ȁ[�񂩓������ρB�v����

		//==========================================================//

		this->SetEyepos(eyepos);	/* ����m_camera�̊֐����`�Ƃ����Ӗ������A*/
		this->SetRefpos(refpos);	/* ����͍����̊֐��Ȃ̂ŁAthis�ő�p�B����this->�͏ȗ��� */


	}


	//=======================FPS������==========================//

	if (m_isFPS)
	{
		// �g���K�[��true�Ȃ�FPS�J�����ݒ�
		// ���_,�Q�Ɠ_
		Vector3 eyepos, refpos;

		// �Q�Ɠ_�ɂ��Ă��ꏊ�i���@�ꏊ�j�����_�ɂȂ�΂����̂ŁArefpos��eyepos�ɕς��A�����𒲐�
		eyepos = m_targetPos + Vector3(0, 0.5f, 0);
		// ���_�ƎQ�Ɠ_�̍����x�N�g���BTPS�Ƌt
		Vector3 cameraV(0, 0, -5);

		// ���@�̌��ɉ�荞�ނ��߂̉�]�s��𐶐�
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);

		// �����x�N�g������]������
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// �J�������W���v�Z�B�v�Z����͎̂Q�Ɠ_���W
		refpos = eyepos + cameraV;

		/* Game.cpp�́A���@�`�敔���������΂�肻����ۂ� */

		this->SetEyepos(eyepos);	/* ����m_camera�̊֐����`�Ƃ����Ӗ������A*/
		this->SetRefpos(refpos);	/* ����͍����̊֐��Ȃ̂ŁAthis�ő�p�B����this->�͏ȗ��� */

	}

	//============================================================//

	// ���N���X�̍X�V
	Camera::Update();

}


////----------------------------------------------------------------------
////! @�֐����FSetTargetPos
////!
////! @�����F���@�̍��W���m�F����
////!
////! @�����F���@�̍��W(&Vector3)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_targetPos = targetPos;
}

////----------------------------------------------------------------------
////! @�֐����FSetTargetAngle
////!
////! @�����F���@�̊p�x���m�F����
////!
////! @�����F���@�̊p�x(&float)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void FollowCamera::SetTargetAngle(const float & targetAngle)
{
	m_targetAngle = targetAngle;
}



////----------------------------------------------------------------------
////! @�֐����FSetKeyboard
////!
////! @�����F�L�[�{�[�h�̒l���O������󂯎��
////!
////! @�����F�L�[�{�[�h�̏��(Keyboard)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}


