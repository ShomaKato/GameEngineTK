///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/05
//*
//*	@���O		Enemy.cpp
//*
//* @����		�G�l�~�[�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// �w�b�_�̃C���N���[�h
#include "pch.h"
#include "enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


////----------------------------------------------------------------------
////! @�֐����FEnemy
////!
////! @�����F�v���C���N���X�̃R���X�g���N�^
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Enemy::Enemy()
{
	// �������֐�
	InitializeEnemy();
}


////----------------------------------------------------------------------
////! @�֐����F~Enemy
////!
////! @�����F�v���C���N���X�̃f�X�g���N�^
////!
////! @�����F���݂��Ȃ�
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Enemy::~Enemy()
{
}

////----------------------------------------------------------------------
////! @�֐����FInitializeEnemy
////!
////! @�����F�G�l�~�[�I�u�W�F�N�g�̏�����
////!
////! @�����F�Ȃ��ivoid�j
////!
////! @�߂�l�F�Ȃ��ivoid�j
////----------------------------------------------------------------------
void Enemy::InitializeEnemy()
{

	// ���@�p�[�c�̓ǂݍ���
	enemy.resize(ROBOT_PARTS_NUM);
	enemy[ROBOT_PARTS_CRAWLER].LoadModel(L"Resources/robo_crawler.cmo");
	enemy[ROBOT_PARTS_HIP].LoadModel(L"Resources/robo_hip.cmo");
	enemy[ROBOT_PARTS_RWING].LoadModel(L"Resources/robo_wing.cmo");
	enemy[ROBOT_PARTS_LWING].LoadModel(L"Resources/robo_wing.cmo");
	enemy[ROBOT_PARTS_BODY].LoadModel(L"Resources/robo_body.cmo");
	enemy[ROBOT_PARTS_HEAD].LoadModel(L"Resources/robot.cmo");


	//* �p�[�c�̐e�q�֌W���Z�b�g
	// �����̐e���L���^�s����
	enemy[ROBOT_PARTS_HIP].SetParent(
		&enemy[ROBOT_PARTS_CRAWLER]);
	// �����̐e��������
	enemy[ROBOT_PARTS_BODY].SetParent(
		&enemy[ROBOT_PARTS_HIP]);
	// �����̐e��������
	enemy[ROBOT_PARTS_RWING].SetParent(
		&enemy[ROBOT_PARTS_BODY]);
	enemy[ROBOT_PARTS_LWING].SetParent(
		&enemy[ROBOT_PARTS_BODY]);
	// �����̐e��������
	enemy[ROBOT_PARTS_HEAD].SetParent(
		&enemy[ROBOT_PARTS_BODY]);


	//* �e����̃I�t�Z�b�g�i���W�̂��炵���j���Z�b�g
	//* ��
	// ���W����
	enemy[ROBOT_PARTS_HIP].SetTranslation(
		Vector3(0, 0.4f, 0));
	// �p�x����
	enemy[ROBOT_PARTS_HIP].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	enemy[ROBOT_PARTS_HIP].SetScale(
		Vector3(1, 1, 1));

	//* ��
	// ���W����
	enemy[ROBOT_PARTS_BODY].SetTranslation(
		Vector3(0, 1.1f, 0));
	// �p�x����
	enemy[ROBOT_PARTS_BODY].SetRotation(
		Vector3(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f)));
	// �T�C�Y����
	enemy[ROBOT_PARTS_BODY].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	//* �E��
	// ���W����
	enemy[ROBOT_PARTS_RWING].SetTranslation(
		Vector3(0.5f, -0.3f, 0.85f));
	// �p�x����
	enemy[ROBOT_PARTS_RWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));

	//* ����
	// ���W����
	enemy[ROBOT_PARTS_LWING].SetTranslation(
		Vector3(-0.5f, -.4f, 0.85f));
	// �p�x����	/* Z(�E����E��)��Y(�E���琳��)��X(�����獶��)�̏��ŉ�]�H */ �W�J��X
	enemy[ROBOT_PARTS_LWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));


	//* ��
	// ���W����
	enemy[ROBOT_PARTS_HEAD].SetTranslation(
		Vector3(0, 0.9f, 0));
	// �p�x����
	enemy[ROBOT_PARTS_HEAD].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	enemy[ROBOT_PARTS_HEAD].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	// �T�C���p�����̏�����
	m_sinAngle = 0.0f;


	// �����z�u(���K�������_��)
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	// ��b�^�C�}�[�̏�����
	m_timer = 60;

	// �ڕW�p�x�̏�����
	m_DistAngle = 0.0f;

}

////----------------------------------------------------------------------
////! @�֐����FUpdateEnemy
////!
////! @�����F���t���[���̍X�V����
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Enemy::UpdateEnemy()
{

	//// �X�y�[�X�L�[�����Ɣ�񂾂�Ă�����
	//if (key.Space)
	//{
	//	// �t���O��؂�ւ�
	//	m_isLanding = !m_isLanding;
	//}

	//if (m_isLanding)
	//{
	//	// ���łȂ��Ȃ猳�̈ʒu�ɖ߂�
	//	enemy[ROBOT_PARTS_BODY].SetTranslation(
	//		Vector3(0, 1.1f, 0));

	//	enemy[ROBOT_PARTS_RWING].SetTranslation(
	//		Vector3(0.5f, -0.3f, 0.85f));
	//	enemy[ROBOT_PARTS_RWING].SetRotation(
	//		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
	//	enemy[ROBOT_PARTS_RWING].SetScale(
	//		Vector3(1, 1, 1));

	//	enemy[ROBOT_PARTS_LWING].SetTranslation(
	//		Vector3(-0.5f, -0.3f, 0.85f));
	//	enemy[ROBOT_PARTS_LWING].SetRotation(
	//		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
	//	enemy[ROBOT_PARTS_LWING].SetScale(
	//		Vector3(1, 1, 1));

	//}
	//if (!m_isLanding)
	//{
	//	// ���ł���Ȃ�ʒu�Ɗp�x��ύX
	//	m_sinAngle += 0.05f;

	//	enemy[ROBOT_PARTS_BODY].SetTranslation(
	//		Vector3(0, 2.1f + (sinf(m_sinAngle) / 10), 0));

	//	enemy[ROBOT_PARTS_RWING].SetTranslation(
	//		Vector3(1.5f, 0.6f, 0.9f));
	//	enemy[ROBOT_PARTS_RWING].SetRotation(
	//		Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
	//	enemy[ROBOT_PARTS_RWING].SetScale(
	//		Vector3(1.2f, 1.2f, 1.2f));
	//	enemy[ROBOT_PARTS_LWING].SetTranslation(
	//		Vector3(-1.5f, 0.6f, 0.9f));
	//	enemy[ROBOT_PARTS_LWING].SetRotation(
	//		Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
	//	enemy[ROBOT_PARTS_LWING].SetScale(
	//		Vector3(1.2f, 1.2f, 1.2f));
	//}


	//// A�L�[��������Ă�����
	//if (key.A)
	//{
	//	// ���@�̊p�x����]������
	//	//tank_angle += 0.03f;
	//	float angle = enemy[0].GetRotation().y;
	//	enemy[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	//}

	//// D�L�[��������Ă�����
	//if (key.D)
	//{
	//	// ���@�̊p�x����]������
	//	float angle = enemy[0].GetRotation().y;
	//	enemy[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	//}

	//// W�L�[��������Ă�����
	//if (key.W)
	//{
	//	// �ړ��ʂ̃x�N�g��
	//	Vector3 moveV(0, 0, -0.1f);
	//	// �ړ��ʃx�N�g�������@�̊p�x����]������
	//	//moveV = Vector3::TransformNormal(moveV, tank_world);
	//	float angle = enemy[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//	// ���@�̍��W���ړ�������
	//	Vector3 pos = enemy[0].GetTranslation();
	//	enemy[0].SetTranslation(pos + moveV);
	//}

	//// S�L�[��������Ă�����
	//if (key.S)
	//{
	//	// �ړ��ʂ̃x�N�g��
	//	Vector3 moveV(0, 0, +0.1f);
	//	// �ړ��ʃx�N�g�������@�̊p�x����]������
	//	float angle = enemy[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//	// ���@�̍��W���ړ�������
	//	Vector3 pos = enemy[0].GetTranslation();
	//	enemy[0].SetTranslation(pos + moveV);
	//}



	// AI
	m_timer--;
	if (m_timer < 0)
	{
		// �J�E���g��0�ɒB������
		m_timer = 60;

		// �ڕW�p�x��ύX
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		
	}




	// vector�R���e�i��for���ŁA�S�p�[�c�̍X�V�������s��
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Update();
	}

}

////----------------------------------------------------------------------
////! @�֐����FRenderEnemy
////!
////! @�����F�I�u�W�F�N�g��`�悷��
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Enemy::RenderEnemy()
{
	// vector�R���e�i��for���ŁA�S�p�[�c�̕`�揈�����s��
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Draw();
	}
}

