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

	this->SetTrans(pos);

	// ��b�^�C�}�[�̏�����
	m_Timer = 60;

	// �ڕW�p�x�̏�����
	m_DistAngle = 0.0f;



	//// 6/19
	//{
	//	// �����蔻���ݒu
	//	m_collisionNode.Initialize();
	//	// �����蔻��̕\����\���t���O��OFF
		isCollisionVisible = false;
	//}
	m_collisionNode.Initialize();
	m_collisionNode.SetParent(&enemy[ROBOT_PARTS_BODY]);
	//* ���SetParent��update�ł̂ݍs���Ă������A�������ł����Ȃ��Ɠ����蔻�肪�������_�ɗ��Ă��܂��B
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
	// ������Ă���L�[������
	Keyboard::State key = Keyboard->GetState();
	// �L�[�{�[�h�̏�Ԏ擾�i�g���K�[�p�j
	Keyboard::State keyboardstate = Keyboard->GetState();
	keyboardTracker.Update(keyboardstate);

	// AI		/* �搶�̊ۃp�N���Ȃ̂ŁA�悭�������Ă������� */
	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;
		// -0.5�`+0.5�̗���
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;

		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}

	{
		// �p�x����]������
		Vector3 rotv = enemy[0].GetRotation();

		float angle = m_DistAngle - rotv.y;

		// 180�x�𒴂��Ă����ꍇ�A�t���̕����߂�
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}

		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// ���
		rotv.y += angle * 0.05f;

		SetRot(rotv);
	}

	// �@�̂������Ă�������ɐi��
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.05f);
		// �ړ��ʃx�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = enemy[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�������
		Vector3 pos = enemy[0].GetTranslation();
		enemy[0].SetTranslation(pos + moveV);
	}


	// 6/19
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::O))
	{
		// O�L�[�������瓖���蔻��̕\����\���t���O��OFF
		isCollisionVisible = !isCollisionVisible;
	}


	Calc();

}

////----------------------------------------------------------------------
////! @�֐����FCalc
////!
////! @�����F�I�u�W�F�N�g��`�悷��
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Enemy::Calc()
{
	// vector�R���e�i��for���ŁA�S�p�[�c�̍X�V�������s��
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Update();
	}

	// 6/19
	{
		// �����蔻���ݒu
		m_collisionNode.Update();
		// �����蔻���e�ۃ��f���ɐe�q�t
		m_collisionNode.SetParent(&enemy[ROBOT_PARTS_BODY]);
		m_collisionNode.SetTrans(Vector3(0, 0.5f, 0));		/* �D���Ȉʒu�ɒ��� */
		m_collisionNode.SetLocalRadius(1.5f);				/* �D���ȑ傫���ɒ��� */


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


	// 6/19
	if (isCollisionVisible)
	{
		// �e�ۗp�̓����蔻���ݒu
		m_collisionNode.Draw();
	}
}


const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return enemy[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return enemy[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	enemy[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	enemy[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return enemy[0].GetWorld();
}







////----------------------------------------------------------------------
////! @�֐����FSetKeyboard
////!
////! @�����F�L�[�{�[�h��n���֐�
////!
////! @�����F���͂��ꂽ�L�[���(Keyboard* key)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Enemy::SetKeyboard(DirectX::Keyboard * key)
{
	Keyboard = key;
}
