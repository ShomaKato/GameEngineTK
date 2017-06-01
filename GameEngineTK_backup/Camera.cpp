///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/05/11
//*
//*	@���O		Camera.cpp
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// �w�b�_�t�@�C���̃C���N���[�h
#include "DIrectXMath.h"
#include "Camera.h"

// ���O���
using namespace DirectX;			/* XMConvertToRadians */
using namespace DirectX::SimpleMath;


///////////////////////////////
///		�����o�֐��̒�`	///
///////////////////////////////
////----------------------------------------------------------------------
////! @�֐����FCamera
////!
////! @�����F�R���X�g���N�^
////!
////! @�����F�Ȃ�
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Camera::Camera(int width, int height)
{
	// �����o�ϐ��̏������i�r���[�s��֘A�j
	m_eyepos = Vector3(0, 0, 0.5f);		/* Vector3�𔲂����Ă����̂ŃG���[���Ă܂��� */
	m_refpos = Vector3(0, 0, 0);
	m_Upvec = Vector3(0, 1, 0);
	// �r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_Upvec);



	// ����p�̏������i�v���W�F�N�V�����s��֘A�j
	m_fovY = XMConvertToRadians(60.0f);	/* DirectX�𖼑O��Ԃ��Ȃ��ƃ_���BSimpleMath�ł͂Ȃ� */
	m_aspect = (float)width /height;
	m_nearclip = 0.1f;
	m_farclip = 1000.0f;


	// �ˉe�s��̓������e�i�����j
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);
}

////----------------------------------------------------------------------
////! @�֐����F~Camera
////!
////! @�����F�f�X�g���N�^
////!
////! @�����F���݂��Ȃ�
////!
////! @�߂�l�F���݂��Ȃ�
////----------------------------------------------------------------------
Camera::~Camera()
{
}

////----------------------------------------------------------------------
////! @�֐����FUpdate
////!
////! @�����F�J�����̍X�V����
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::Update()
{
	// �r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_Upvec);
	// �ˉe�s��̓������e�i�����j
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

	/* ��L��A�ǂ����Update�ł��������Ȃ��Ə�ɂ��Ă���J�����ɂȂ�Ȃ� */
}






////----------------------------------------------------------------------
////! @�֐����FSetEyepos
////!
////! @�����F���_[eyepos]��ݒ肵�ăZ�b�g����
////!
////! @�����F���_(Vector3)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetEyepos(Vector3 eyepos)
{
	m_eyepos = eyepos;
}

////----------------------------------------------------------------------
////! @�֐����FSetRefpos
////!
////! @�����F�����_[refpos]��ݒ肵�ăZ�b�g����
////!
////! @�����F�����_(Vector3)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetRefpos(Vector3 refpos)
{
	m_refpos = refpos;
}

////----------------------------------------------------------------------
////! @�֐����FSetUpvec
////!
////! @�����F������x�N�g��[Upvec]��ݒ肵�ăZ�b�g����
////!
////! @�����F������x�N�g��(Vector3)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetUpvec(Vector3 Upvec)
{
	m_Upvec = Upvec;
}

////----------------------------------------------------------------------
////! @�֐����FGetViewMatrix
////!
////! @�����F�r���[�s����擾����
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�r���[�s��(Matrix)
////----------------------------------------------------------------------
const Matrix& Camera::GetView()
{
	return m_view;
}






////----------------------------------------------------------------------
////! @�֐����FSetFovY
////!
////! @�����F�������������ݒ肵�ăZ�b�g����
////!
////! @�����F�Ȃ�(float)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

////----------------------------------------------------------------------
////! @�֐����FSetAspect
////!
////! @�����F��ʔ䗦��ݒ肵�ăZ�b�g����
////!
////! @�����F�Ȃ�(float)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

////----------------------------------------------------------------------
////! @�֐����FSetNearclip
////!
////! @�����F�j�A�N���b�v��ݒ肵�ăZ�b�g����
////!
////! @�����F�Ȃ�(float)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetNearclip(float nearclip)
{
	m_nearclip = nearclip;
}

////----------------------------------------------------------------------
////! @�֐����FSetFarclip
////!
////! @�����F�t�@�[�N���b�v��ݒ肵�ăZ�b�g����
////!
////! @�����F�Ȃ�(float)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Camera::SetFarclip(float farclip)
{
	m_farclip = farclip;
}

////----------------------------------------------------------------------
////! @�֐����FGetProjectionMatrix
////!
////! @�����F�ˉe�s����擾����
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�ˉe�s��(Matrix)
////----------------------------------------------------------------------
const Matrix& Camera::GetProj()
{
	return m_proj;
}