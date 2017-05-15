///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/05/11
//*
//*	@���O		Camera.h
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include <d3d11.h>
#include <SimpleMath.h>



class Camera
{
public:
	// �����o�֐��͂����� //
	Camera(int width,int height);
	virtual ~Camera();

	// �J�����̍X�V�֐�
	virtual void Update();


	// ���_���Z�b�g���邽�߂̊֐�
	void SetEyepos(DirectX::SimpleMath::Vector3 eyepos);

	// �����_���Z�b�g���邽�߂̊֐�
	void SetRefpos(DirectX::SimpleMath::Vector3 refpos);

	// ������x�N�g�����Z�b�g���邽�߂̊֐�
	void SetUpvec(DirectX::SimpleMath::Vector3 Upvec);

	//// �r���[�s��𐶐����邽�߂̊֐�
	//void SetViewMatrix();					/* ��O��ύX����΁AUpdate�ŕς��̂ŕs�v */

	// �r���[�s����擾����֐�
	const DirectX::SimpleMath::Matrix& GetView();
	/* const�Q�Ɠn���ɂ����ق����y���Ȃ�炵�� */



	// ������������p���Z�b�g���邽�߂̊֐�
	void SetFovY(float fovY);

	// �����_���Z�b�g���邽�߂̊֐�
	void SetAspect(float aspect);

	// ��O�̕\�����E�i�j�A�N���b�v�j���Z�b�g���邽�߂̊֐�
	void SetNearclip(float nearclip);

	// ���̕\�����E�i�t�@�[�N���b�v�j���Z�b�g���邽�߂̊֐�
	void SetFarclip(float farclip);

	// �v���W�F�N�V�����s����擾����֐�
	const DirectX::SimpleMath::Matrix& GetProj();
	/* ��ɓ��� */




protected:
	// �ޗ��͂����Ƀ����o�ϐ��Ƃ��ď��� //


	//*==============�r���[�s��i�J�����̐ݒ�p�s��j

	// �r���[�s���ϊ�
	/* ���_�i�ǂ����猩�邩�j */
	DirectX::SimpleMath::Vector3 m_eyepos;
	/* �����_�i�ǂ������邩�B0,0,0���ƌ��_�Ɂj */
	DirectX::SimpleMath::Vector3 m_refpos;
	/* ������x�N�g���i�ǂ��炪��ʏォ�j */
	DirectX::SimpleMath::Vector3 m_Upvec;

	// �r���[�s��̐���
	DirectX::SimpleMath::Matrix m_view;





	//*==============�v���W�F�N�V�����s��i�J�����̒����p�s��j

	/* ������������p�i60�x�܂ŉ�ʂɎ��߂�A�Ƃ����Ӗ��Ɂj */
	float m_fovY;
	/* �c���̔䗦�Bfloat�ϊ�������������c���ł����H */
	float m_aspect;	
	/* ��O�̕\�����E���� */
	float m_nearclip;	
	/* ���̕\�����E���� */
	float m_farclip;	
	// �ˉe�s��̓������e�i�����j
	DirectX::SimpleMath::Matrix m_proj;


	//* MEMO
	//  ���_ == �Q�Ɠ_���Ɨ�����B
	//  ���̓�̍��ŃJ�����̊p�x�����܂邽�߁B
};
