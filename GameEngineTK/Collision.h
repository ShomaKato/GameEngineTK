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
#include <Windows.h>		/* �����A������Ƃ��ƃG���[�����邱�ƂƂ�����炵�� */
#include <d3d11_1.h>
#include <SimpleMath.h>


//namespace Collision
//* �{���́��̂悤�ɁA���C�u��������Ă����B���Ƃ��ƃL�����Ȃ��̂Ŋ���




// ����̏Փ˔���N���X
class Sphere
{
public:			/* �f�[�^�o���N�Ƃ��Ďg�������ŁA�֐��Ƃ��������������͂Ȃ��̂Ńp�u���b�N */
	// ���S���W
	DirectX::SimpleMath::Vector3 Center;
	// ���a
	float Radius;
	// �R���X�g���N�^
	Sphere()
	{
		// �f�t�H���g�Ŕ��a�ꃁ�[�g��
		Radius = 1.0f;		/* ���̏��������s�����������߂ɃN���X���B�{���Ȃ�X�g���N�g�ł��� */
	}
};

// ����(Sphere�Ƒg�ݍ��킹�ăJ�v�Z���ɂ���)
class Segment
{
public:
	// �n�_
	DirectX::SimpleMath::Vector3 Start;
	// �I�_
	DirectX::SimpleMath::Vector3 End;



};


// �O���[�o���֐�
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB);