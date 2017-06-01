///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/01
//*
//*	@���O		Player.cpp
//*
//* @����		�v���C���[�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// �w�b�_�̃C���N���[�h
#include "pch.h"
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


////----------------------------------------------------------------------
////! @�֐����FPlayer
////!
////! @�����F�v���C���N���X�̃R���X�g���N�^
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Player::Player()
{

	// ���@�p�[�c�̓ǂݍ���
	player.resize(ROBOT_PARTS_NUM);
	player[ROBOT_PARTS_CRAWLER].LoadModel(L"Resources/robo_crawler.cmo");
	player[ROBOT_PARTS_HIP].LoadModel(L"Resources/robo_hip.cmo");
	player[ROBOT_PARTS_RWING].LoadModel(L"Resources/robo_wing.cmo");
	player[ROBOT_PARTS_LWING].LoadModel(L"Resources/robo_wing.cmo");
	player[ROBOT_PARTS_BODY].LoadModel(L"Resources/robo_body.cmo");
	player[ROBOT_PARTS_HEAD].LoadModel(L"Resources/robot.cmo");


	//* �p�[�c�̐e�q�֌W���Z�b�g
	// �����̐e���L���^�s����
	player[ROBOT_PARTS_HIP].SetParent(
		&player[ROBOT_PARTS_CRAWLER]);
	// �����̐e��������
	player[ROBOT_PARTS_BODY].SetParent(
		&player[ROBOT_PARTS_HIP]);
	// �����̐e��������
	player[ROBOT_PARTS_RWING].SetParent(
		&player[ROBOT_PARTS_BODY]);
	player[ROBOT_PARTS_LWING].SetParent(
		&player[ROBOT_PARTS_BODY]);
	// �����̐e��������
	player[ROBOT_PARTS_HEAD].SetParent(
		&player[ROBOT_PARTS_BODY]);


	//* �e����̃I�t�Z�b�g�i���W�̂��炵���j���Z�b�g
	//* ��
	// ���W����
	player[ROBOT_PARTS_HIP].SetTranslation(
		Vector3(0, 0.4f, 0));
	// �p�x����
	player[ROBOT_PARTS_HIP].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	player[ROBOT_PARTS_HIP].SetScale(
		Vector3(1, 1, 1));

	//* ��
	// ���W����
	player[ROBOT_PARTS_BODY].SetTranslation(
		Vector3(0, 1.1f, 0));
	// �p�x����
	player[ROBOT_PARTS_BODY].SetRotation(
		Vector3(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f)));
	// �T�C�Y����
	player[ROBOT_PARTS_BODY].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	//* �E��
	// ���W����
	player[ROBOT_PARTS_RWING].SetTranslation(
		Vector3(0.5f, -0.3f, 0.85f));
	// �p�x����
	player[ROBOT_PARTS_RWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));

	//* ����
	// ���W����
	player[ROBOT_PARTS_LWING].SetTranslation(
		Vector3(-0.5f, -.4f, 0.85f));
	// �p�x����	/* Z(�E����E��)��Y(�E���琳��)��X(�����獶��)�̏��ŉ�]�H */ �W�J��X
	player[ROBOT_PARTS_LWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));


	//* ��
	// ���W����
	player[ROBOT_PARTS_HEAD].SetTranslation(
		Vector3(0, 0.9f, 0));
	// �p�x����
	player[ROBOT_PARTS_HEAD].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	player[ROBOT_PARTS_HEAD].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	// �L�[�{�[�h������������
	Keyboard = nullptr;

	// �T�C���p�����̏�����
	m_sinAngle = 0.0f;

}


////----------------------------------------------------------------------
////! @�֐����F~Player
////!
////! @�����F�v���C���N���X�̃f�X�g���N�^
////!
////! @�����F���݂��Ȃ�
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Player::~Player()
{
}

////----------------------------------------------------------------------
////! @�֐����FUpdatePlayer
////!
////! @�����F���t���[���̍X�V����
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Player::UpdatePlayer()
{
	Keyboard::State key = Keyboard->GetState();


	// �X�y�[�X�L�[�����Ɣ�񂾂�Ă�����
	if (key.Space)
	{
		// �t���O��؂�ւ�
		m_isLanding = !m_isLanding;
	}

	if (m_isLanding)
	{
		player[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 1.1f, 0));

		// ���łȂ��Ȃ猳�̈ʒu�ɖ߂�
		player[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(0.5f, -0.3f, 0.85f));
		player[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		player[ROBOT_PARTS_RWING].SetScale(
			Vector3(1, 1, 1));

		player[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-0.5f, -0.3f, 0.85f));
		player[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		player[ROBOT_PARTS_LWING].SetScale(
			Vector3(1, 1, 1));

	}
	if (!m_isLanding)
	{
		// ���ł���Ȃ�ʒu�Ɗp�x��ύX
		m_sinAngle += 0.05f;

		player[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 2.1f + (sinf(m_sinAngle) / 10), 0));

		player[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(1.5f, 0.6f, 0.9f));
		player[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		player[ROBOT_PARTS_RWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));
		player[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-1.5f, 0.6f, 0.9f));
		player[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		player[ROBOT_PARTS_LWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));
	}


	// A�L�[��������Ă�����
	if (key.A)
	{
		// ���@�̊p�x����]������
		//tank_angle += 0.03f;
		float angle = player[0].GetRotation().y;
		player[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// D�L�[��������Ă�����
	if (key.D)
	{
		// ���@�̊p�x����]������
		float angle = player[0].GetRotation().y;
		player[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// W�L�[��������Ă�����
	if (key.W)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��ʃx�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = player[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�������
		Vector3 pos = player[0].GetTranslation();
		player[0].SetTranslation(pos + moveV);
	}

	// S�L�[��������Ă�����
	if (key.S)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, +0.1f);
		// �ړ��ʃx�N�g�������@�̊p�x����]������
		float angle = player[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�������
		Vector3 pos = player[0].GetTranslation();
		player[0].SetTranslation(pos + moveV);
	}

	// vector�R���e�i��for���ŁA�S�p�[�c�̍X�V�������s��
	for (std::vector<Obj3d>::iterator it = player.begin();
		it != player.end();
		it++)
	{
		it->Update();
	}

}

////----------------------------------------------------------------------
////! @�֐����FRenderPlayer
////!
////! @�����F�I�u�W�F�N�g��`�悷��
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Player::RenderPlayer()
{
	// vector�R���e�i��for���ŁA�S�p�[�c�̕`�揈�����s��
	for (std::vector<Obj3d>::iterator it = player.begin();
		it != player.end();
		it++)
	{
		it->Draw();
	}
}

////----------------------------------------------------------------------
////! @�֐����FSetKeyboard
////!
////! @�����F�L�[�{�[�h�̏���n���֐�
////!
////! @�����F���͂��ꂽ�L�[���(Keyboard* key)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Player::SetKeyboard(DirectX::Keyboard * key)
{
	Keyboard = key;
}

////----------------------------------------------------------------------
////! @�֐����FGetPlayerPos
////!
////! @�����F�Q�Ɠ_�i�v���C���̈ʒu�j���󂯎��֐�
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3 Player::GetPlayerTranslation()
{
	return player[0].GetTranslation();
}

////----------------------------------------------------------------------
////! @�֐����FGetRotateY
////!
////! @�����F���_�i�J�����̈ʒu�j���󂯎��֐�
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
float Player::GetPlayerRotationY()
{
	return player[0].GetRotation().y;
}
