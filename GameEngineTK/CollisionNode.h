///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/15
//*
//*	@���O		Collision.h
//*
//* @����		�Փ˔���̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�[�̃C���N���[�h
#include "Collision.h"
#include "Obj3d.h"

// �����蔻��̐e�m�[�h
class CollisionNode
{
public:		/* �����蔻��̋��ʋ@�\�͂����� */
	// ������
	virtual void Initialize() = 0;	/* virtual����=0�����珃�����z�֐��B���g�͌p����ɔC���� */
	// ���t���[���X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;

	// �e�m�[�h(Objd�^)��ݒ�
	void SetParent(Obj3d * parent);

	// �e����̃I�t�Z�b�g�i���W�̃Y���j��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3 trans) { m_Trans = trans; }

protected:
	// �f�o�b�O�\���p�I�u�W�F�N�g�i�����蔻�肪������悤�Ɂj
	Obj3d m_obj;
	// �e����̃I�t�Z�b�g�i���W�̃Y���j
	DirectX::SimpleMath::Vector3 m_Trans;

};


// ���̓����蔻��m�[�h
class SphereNode : public CollisionNode, public Sphere
{
public:
	// �R���X�g���N�^
	SphereNode();
	// ������
	void Initialize();
	// ���t���[���X�V
	void Update();
	// �`��
	void Draw();

	// �g�k�O�̔��a���Z�b�g
	void SetLocalRadius(const float radius) { m_localRadius = radius; }
	// �g�k�O�̔��a���Z�b�g

protected:
	// �X�P�[�����O�Ŋg�k�����O�̓����蔻��̔��a
	float m_localRadius;

};