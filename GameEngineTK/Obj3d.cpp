///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/05/22
//*
//*	@���O		Obj3d.cpp
//*
//* @����		3D�I�u�W�F�N�g�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------

// �w�b�_�̃C���N���[�h
#include "pch.h"
#include "Obj3d.h"

// ���O���
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;



// �ÓI�����o�ϐ��̎���

// �f�o�C�X/* cmo�̓ǂݍ��݂ɕK�{ */
Microsoft::WRL::ComPtr<ID3D11Device>			Obj3d::m_d3dDevice;
// �f�o�C�X�R���e�L�X�g/* ���f���`��ɕK�{ */
Microsoft::WRL::ComPtr<ID3D11DeviceContext>		Obj3d::m_d3dContext;

// �J�����i�r���[�s��Ǝˉe�s�񂪓����Ă�j
Camera*											Obj3d::m_camera;

// �ėp�`��X�e�[�g
std::unique_ptr<DirectX::CommonStates>			Obj3d::m_states;

// �G�t�F�N�g�t�@�N�g��/* �����킩��񂯂ǕK�{ */
std::unique_ptr<DirectX::EffectFactory>			Obj3d::m_factory;

/* Obj3d::���Ȃ��ƁA�����̃O���[�o���ϐ���ԂɂȂ�̂Œ��� */




////----------------------------------------------------------------------
////! @�֐����FInitializeStatic
////!
////! @�����F�ÓI�����o�ϐ��̏���������
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dContext = d3dContext;
	m_d3dDevice = d3dDevice;
	m_camera = camera;


	// �ėp�`��X�e�[�g�𐶐�
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��		/* ��������e�N�X�`����ǂݍ��݂܂���A�Ƃ����Ӗ��� */
	m_factory->SetDirectory(L"Resources");
}





////----------------------------------------------------------------------
////! @�֐����FObj3d
////!
////! @�����F3D���f���̏���������
////!
////! @�����F�Ȃ�(void)
////!
////! @�߂�l�F�Ȃ�(void)
////----------------------------------------------------------------------
Obj3d::Obj3d()
{
	// �X�P�[����1�{���f�t�H���g
	m_scale = Vector3(1, 1, 1);

	// �e��3D�I�u�W�F�N�g���܂��͋�ɂ��Ă���
	m_parent = nullptr;
}


Obj3d::~Obj3d()
{
}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	// ���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory);
}

void Obj3d::Update()
{
	/* �s��̌v�Z */


	// �X�P�[�����O�s��
	Matrix scalemat = Matrix::CreateScale(m_scale);

	// ��]�s��
	/* ���ꂼ��̉�]�������� */
	Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
	Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
	Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
												
	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	// ���s�ړ��s��
	Matrix transmat = Matrix::CreateTranslation(m_translation);


	// ���[���h�s��̍����@�i�e������Ȃ�A�e�̃��[���h�s����Y�ꂸ�Ɂj
	m_world = scalemat * rotmat * transmat;
	// �e������Ȃ�A�e�̍s����|����
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}

}

void Obj3d::Draw()
{
	//* 3D���f���̕`��


	// �J������nullptr�������狭���I��
	assert(m_camera);

	// ���f�������݂��Ă���Ȃ�A�����`�悷��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states,
			Matrix::Identity,
			m_camera->GetView(),
			m_camera->GetProj());
	}

}
