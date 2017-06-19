///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/06/15
//*
//*	@���O		Collision.cpp
//*
//* @����		�Փ˔���̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// �w�b�_�̃C���N���[�h
#include "pch.h"
#include "Collision.h"


// ���O���
using namespace DirectX::SimpleMath;


// �O���[�o���֐�
////----------------------------------------------------------------------
////! @�֐����FCheckSphere2Sphere
////!
////! @�����F���Ƌ��̓����蔻����m�F
////!
////! @�����F��A(Sphere)�Ƌ�B(Sphere)
////!
////! @�߂�l�F�q�b�g�������ۂ�
////----------------------------------------------------------------------
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// ��A�Ƌ�B�̒��S���W�̍����v�Z����
	Vector3 sub = sphereA.Center - sphereB.Center;


	// �����̓��
	float distanceSquare;
	// �O�����̒藝�Ńx�N�g���̒������v�Z����
	float distance = sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
	/* �����x�N�g���̒����͓�_�Ԃ̋��� */


	// ���a�̘a�̓��
	float radiusSquare;

	radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;


	// ���������a�̘a���傫����Γ������Ă��Ȃ�
	if (distance > sphereA.Radius + sphereB.Radius)
	{
		return false;
	}

	return true;
}


