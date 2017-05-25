///-------------------------------------------------------------------
//*
//*	@�쐬��		2017/05/22
//*
//*	@���O		Obj3d.h
//*
//* @����		3D�I�u�W�F�N�g�̃N���X
//*
//*	@����		���� ���n
//*
///-------------------------------------------------------------------
// ���d�C���N���[�h�̖h�~
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	// �ÓI�����o�ϐ��̏��������s���ϐ��i�O������n�����͈̂����Ɂj
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>	d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
		Camera* camera);
	/* �n���̂̓f�o�C�X�A�f�o�C�X�R���e�L�X�g�A�J���� */

private:
	//==========�ÓI�����o�ϐ��i�C���X�^���X���������񂠂��Ă������͈�j==========//

	// �f�o�C�X		/* cmo�̓ǂݍ��݂ɕK�{ */
	static Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	// �f�o�C�X�R���e�L�X�g		/* ���f���`��ɕK�{ */
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

	// �J�����i�r���[�s��Ǝˉe�s�񂪓����Ă�j
	static Camera* m_camera;
																
	// �ėp�`��X�e�[�g
	static std::unique_ptr<DirectX::CommonStates> m_states;

	// �G�t�F�N�g�t�@�N�g��/* �����킩��񂯂ǕK�{ */
	static std::unique_ptr<DirectX::EffectFactory> m_factory;




public:
	Obj3d();	/* �S���Z�b�^�[�Q�b�^�[�Ŏ󂯎��̂ŁA�f�t�H���g�R���X�g���N�^�ł��� */
	~Obj3d();

	// ���f�������[�h
	void LoadModel(const wchar_t* fileName);	/* �t�@�C������CreateFromCMO�̈������p�N�� */

	// �X�V
	void Update();

	// �`��
	void Draw();


	//==========setter==========//
	// �X�P�[�����O(XYZ)	/* �����l�O���ƂO�{�\�����悤�Ƃ���̂ŁA1,1,1�ɂ��Ƃ��Ƃ悵 */
	void SetScale(DirectX::SimpleMath::Vector3&	scale) { m_scale = scale; }

	// ��]�p(Z,Y,X)
	void SetRotation(DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }

	// ���s�ړ�(X,Y,Z)
	void SetTranslation(DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }

	// �e��3D�I�u�W�F�N�g�̃|�C���^
	void SetParent(Obj3d* parent) { m_parent = parent; }

	//==========getter==========//
	// �X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	// ��]�p(Z,Y,X)
	const DirectX::SimpleMath::Vector3 GetRotation() { return m_rotation; }

	// ���s�ړ�(X,Y,Z)
	const DirectX::SimpleMath::Vector3 GetTranslation() { return m_translation; }

	// ���[���h�s��
	const DirectX::SimpleMath::Matrix GetWorld() { return m_world; }

	// �e��3D�I�u�W�F�N�g�̃|�C���^
	const Obj3d* GetParent() { return m_parent; }


private:
	//==========�����o�ϐ�==========//
	// 3D���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3	m_scale;	/* Vector3�͍ŏ�����(0,0,0)�������Ă�̂ŁA */
												/* scale�����͏������ŕς��Ȃ��Ƃ܂����B */
	// ��]�p(Z,Y,X)
	DirectX::SimpleMath::Vector3	m_rotation;

	// ���s�ړ�(X,Y,Z)
	DirectX::SimpleMath::Vector3	m_translation;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix		m_world;

	// �i�e�����݂���Ȃ�΁j�e�p�[�c��3D�I�u�W�F�N�g�A�̃|�C���^
	Obj3d*							m_parent;
};