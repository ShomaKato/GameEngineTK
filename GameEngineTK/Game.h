//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "DebugCamera.h"
#include "Camera.h"
#include "FollowCamera.h"
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"
#include "Player.h"
#include "Enemy.h"
#include "LandShape.h"


static const int ENEMY_NUM = 5;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;


	//* cpp�ŃO���[�o���ϐ��ɂ��Ă������̂��ړ����ă����o�ϐ���
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	////* �`��p�s��̃����o�ϐ�
	//DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	////* �f�o�b�O�J����
	//std::unique_ptr<DebugCamera> m_debugCamera;


	//* �G�t�F�N�g�t�@�N�g��
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//* ���f��			/* ���f���̗e�ꕨ */
	std::unique_ptr<DirectX::Model> m_groundModel;
	//std::unique_ptr<DirectX::Model> m_skydomeModel;
	std::unique_ptr<DirectX::Model> m_PRACTICALballModel;
	std::unique_ptr<DirectX::Model> m_ballModel;
	std::unique_ptr<DirectX::Model> m_robotModel;

	// ��n�߂ɓV�����A������N���X�̌^�ɂ��Ă݂�
	Obj3d m_skydomeModel;


	std::unique_ptr<DirectX::Model> m_potModel;

	//* ���̃��[���h�s��i���K�j
	DirectX::SimpleMath::Matrix m_PRACTICALballWorld;
	//* ���̃��[���h�s��i�{�ԁj
	DirectX::SimpleMath::Matrix m_ballWorld[20];
	//* �e�B�[�|�b�g�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_potWorld[20];


	//* �V���Y�̉�]��
	float rollingAmount;

	//* �L�[�{�[�h�̓o�^�I�ȃT���V���O
	std::unique_ptr<DirectX::Keyboard> keyboard;

	//* ���@�i���{�b�g�j�̍��W
	DirectX::SimpleMath::Vector3 robot_pos;
	//* ���@�̉�]�p
	DirectX::SimpleMath::Vector3 robot_angle;
	////* ���@�̃��[���h�s��P
	//DirectX::SimpleMath::Matrix m_robotWorld;	
	////* ���@�̃��[���h�s��Q
	//DirectX::SimpleMath::Matrix m_robotWorld2;


	// �J����
	std::unique_ptr<FollowCamera> m_camera;

	// �v���C��
	Player* m_player;	/* ���j�[�N�|�C���^�̕������ꐫ�����邩�� */

	// �G�l�~(��������~�����̂Ńx�N�^�[�R���e�i�����)
	//std::vector<Enemy> m_enemy;
	std::vector<std::unique_ptr<Enemy>> m_enemy;

	/* �R���e�i�̒��ɂ����̃G�l�~�[���ƁA�f�t�H���g�R���X�g���N�^���Ȃ��ăG���[�B
	   ���j�[�N�|�C���^�Ȃ炻�ꂪ����Ȃ��̂ő��v�A�炵�� */


	//(7/3) �n�`
	LandShape m_landshape;
};