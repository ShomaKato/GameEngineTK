//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

using Microsoft::WRL::ComPtr;



Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	// �������͂����ɒǉ�����

	// 5/22  //
	// �L�[�{�[�h�̏�����
	keyboard = make_unique<Keyboard>();
	// �J�����̏�����
	m_camera = make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//* �J�����ɃL�[�{�[�h��n��
	m_camera->SetKeyboard(keyboard.get());

	// 3D�I�u�W�F�N�g�̐ÓI�����o��������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());





	// �v���~�e�B�u�o�b�`�̍쐬
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());					/* unique_ptr�̓X�}�[�g�|�C���^�̈��Bdelete�������ł���Ă���� */		
																					/* get()�́A���j�[�N�|�C���^�𕁒ʂ̃|�C���^�ɕϊ����邽�߂̊֐� */

	//// ���[���h�s��̏������H		/* �ǋL�FIdentify�͒P�ʍs��B�܂��܂菉�����ł��� */
	//m_world = Matrix::Identity;

	//// �r���[�s��Ǝˉe�s��̐ݒ�
	//m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
	//	Vector3::Zero, Vector3::UnitY);
	//m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
	//	float(m_outputWidth) / float(m_outputHeight),
	//	0.1f,	
	//	500.f);


	// �x�[�V�b�N�G�t�F�N�g�̍쐬�E������					
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// �v���~�e�B�u�o�b�`�̍쐬�E������
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());

	//// �f�o�b�O�J�����̍쐬
	//m_debugCamera = make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// �G�t�F�N�g�t�@�N�g���𐶐�
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��		/* ��������e�N�X�`����ǂݍ��݂܂���A�Ƃ����Ӗ��� */
	m_factory->SetDirectory(L"Resources");

	// �n�ʃ��f���̓ǂݍ���
	m_groundModel = Model::CreateFromCMO(m_d3dDevice.Get(), 
									L"Resources\\ground200m.cmo", 
									*m_factory);	
	//// �V�����f���̓ǂݍ���
	//m_skydomeModel = Model::CreateFromCMO(m_d3dDevice.Get(),
	//	L"Resources\\SkyDome.cmo",
	//	*m_factory);

	// �V�����f���iObj3d�^�j�̓ǂݍ���
	m_skydomeModel.LoadModel(L"Resources/SkyDome.cmo");


	// �����ׂ��E20��ǂݍ���
	m_ballModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		(L"Resources\\ball.cmo"),
		*m_factory);

	// �e�B�[�|�b�g���ׂ��E20��ǂݍ���
	m_potModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		(L"Resources\\edfPot.cmo"),
		*m_factory);

	// ���@���f���̓ǂݍ���
	//m_robotModel = Model::CreateFromCMO(m_d3dDevice.Get(),
	//	(L"Resources\\robot.cmo"),
	//	*m_factory);

	//rollingAmount = 0.0f;



	// ���@�p�[�c�̓ǂݍ���
	m_ObjPlayer1.resize(ROBOT_PARTS_NUM);
	m_ObjPlayer1[ROBOT_PARTS_CRAWLER].LoadModel(L"Resources/robo_crawler.cmo");
	m_ObjPlayer1[ROBOT_PARTS_HIP].LoadModel(L"Resources/robo_hip.cmo");
	m_ObjPlayer1[ROBOT_PARTS_RWING].LoadModel(L"Resources/robo_wing.cmo");
	m_ObjPlayer1[ROBOT_PARTS_LWING].LoadModel(L"Resources/robo_wing.cmo");
	m_ObjPlayer1[ROBOT_PARTS_BODY].LoadModel(L"Resources/robo_body.cmo");
	m_ObjPlayer1[ROBOT_PARTS_HEAD].LoadModel(L"Resources/robot.cmo");


	//* �p�[�c�̐e�q�֌W���Z�b�g
	// �����̐e���L���^�s����
	m_ObjPlayer1[ROBOT_PARTS_HIP].SetParent(
		&m_ObjPlayer1[ROBOT_PARTS_CRAWLER]);
	// �����̐e��������
	m_ObjPlayer1[ROBOT_PARTS_BODY].SetParent(
		&m_ObjPlayer1[ROBOT_PARTS_HIP]);
	// �����̐e��������
	m_ObjPlayer1[ROBOT_PARTS_RWING].SetParent(
		&m_ObjPlayer1[ROBOT_PARTS_BODY]);
	m_ObjPlayer1[ROBOT_PARTS_LWING].SetParent(
		&m_ObjPlayer1[ROBOT_PARTS_BODY]);
	// �����̐e��������
	m_ObjPlayer1[ROBOT_PARTS_HEAD].SetParent(
		&m_ObjPlayer1[ROBOT_PARTS_BODY]);
	

	//* �e����̃I�t�Z�b�g�i���W�̂��炵���j���Z�b�g
	//* ��
	// ���W����
	m_ObjPlayer1[ROBOT_PARTS_HIP].SetTranslation(
		Vector3(0, 0.4f, 0));
	// �p�x����
	m_ObjPlayer1[ROBOT_PARTS_HIP].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	m_ObjPlayer1[ROBOT_PARTS_HIP].SetScale(
		Vector3(1, 1, 1));

	//* ��
	// ���W����
	m_ObjPlayer1[ROBOT_PARTS_BODY].SetTranslation(
		Vector3(0, 1.1f, 0));
	// �p�x����
	m_ObjPlayer1[ROBOT_PARTS_BODY].SetRotation(
		Vector3(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f)));
	// �T�C�Y����
	m_ObjPlayer1[ROBOT_PARTS_BODY].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	//* �E��
	// ���W����
	m_ObjPlayer1[ROBOT_PARTS_RWING].SetTranslation(
		Vector3(0.5f, -0.3f, 0.85f));
	// �p�x����	/* Z(�ォ�牜)��Y(�E���琳��)��X(�����獶��)�̏��ŉ�]�H */ �W�J��X
	m_ObjPlayer1[ROBOT_PARTS_RWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));

	//* ����
	// ���W����
	m_ObjPlayer1[ROBOT_PARTS_LWING].SetTranslation(
		Vector3(-0.5f, -.4f, 0.85f));
	// �p�x����	/* Z(�E����E��)��Y(�E���琳��)��X(�����獶��)�̏��ŉ�]�H */ �W�J��X
	m_ObjPlayer1[ROBOT_PARTS_LWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));


	//* ��
	// ���W����
	m_ObjPlayer1[ROBOT_PARTS_HEAD].SetTranslation(
		Vector3(0, 0.9f, 0));
	// �p�x����
	m_ObjPlayer1[ROBOT_PARTS_HEAD].SetRotation(
		Vector3(0, 0, 0));
	// �T�C�Y����
	m_ObjPlayer1[ROBOT_PARTS_HEAD].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));


	// �T�C���p�����̏�����
	m_sinAngle = 0.0f;


	//// �f�o�b�O�����p�̍ޗ��̏�����
	//// �R�����X�e�[�g�쐬
	//m_state.reset(new CommonStates(g_pd3dDevice.Get()));


	//=====�����܂łŁA�������ݒ�͊���=====//
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	// ���t���[���X�V�����͂����ɒǉ�����

	// �f�o�b�N�K�����𖈃t���[���X�V
	//m_debugCamera->Update();



	m_camera->SetTargetPos(m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetTranslation());
	m_camera->SetTargetAngle(m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetRotation().y);

	m_camera->Update();

	m_skydomeModel.Update();




	// �r���[�s����擾
	//m_view = m_debugCamera->GetCameraMatrix();


	// ����J�����𖈃t���[���X�V���A�r���[�s��,�ˉe�s����󂯎��
	{// ���@�ɒǏ]����J����
		//m_camera->SetTargetPos(robot_pos);
		//m_camera->SetTargetAngle(robot_angle.y);

		//// �J�����̍X�V
		//m_camera->Update();
		//m_view = m_camera->GetView();
		//m_proj = m_camera->GetProj();
	}	/* eyepos��refpos�������ɂȂ�̂ł��炷 */

	/* ��L�̓S���R�J���������������̂ŗp�ς݂ł��� */

	//=====================���̃��[���h�s����v�Z�i�A�b�v�f�[�g�ł��j==========================//

	//* �X�P�[�����O		/* (�{��) */
	Matrix scalemat = Matrix::CreateScale(0.1f);

	//* ��]				/* Z,X,Y�̏��Ԃň���łȂ���΃_���ȏ�Ƀ��W�A�� */
	// ����
	Matrix rotmatZ = Matrix::CreateRotationZ(XM_PIDIV4);							/* 1/4�� */
	// �p
	Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(15.0f));			/* �x���ւ̕ϊ��֐� */
	// ���ʊp
	Matrix rotmatY = Matrix::CreateRotationY(XM_1DIVPI);							/* ���Ȃ݂�DIV�̕����ׂ����v�Z�ł���炵�� */
	// ��]�s��̍���
	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//* ���s�ړ�	/* (X��,Y��,Z��) */
	Matrix transmat = Matrix::CreateTranslation(1.0f, 2.0f, 0.0f);


	// ���[���h�s��̍���			/* �K��SRT�̏��Ŋ|�����킹�� */
	m_PRACTICALballWorld = scalemat * rotmat * transmat;


	//===========================================================================================//

	//=======================================��̖{��============================================//

	//* ���s�ړ�	/* ���������Ɋ|���ĒZ�k����}�� */
	Matrix transmats1 = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);			/* ��Âɍl�����畽�s�ړ��̍s��͓�ł悩�����̂ŁAfor���̊O�� */
	Matrix transmats2 = Matrix::CreateTranslation(40.0f, 0.0f, 0.0f);
	Matrix rotmats[20];

	for (int i = 0; i < 10; i++)
	{
		// ��]
		Matrix rotmatsZ1 = Matrix::CreateRotationZ(0.0f);
		Matrix rotmatsX1 = Matrix::CreateRotationX(0.0f);
		Matrix rotmatsY1 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i));
		Matrix rotmatsY2 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i));

	
			rotmatsY1 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i) + rollingAmount);
		
			rotmatsY2 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i) - rollingAmount);
		

		rotmats[i] = rotmatsZ1 * rotmatsX1 * rotmatsY1;

		// ���[���h�s��̍���
		m_ballWorld[i] = transmats1 * rotmats[i];
		// 10�`19�͊O��
		rotmats[i +10] = rotmatsY2;
		m_ballWorld[i + 10] = transmats2 * rotmats[i + 10];
	}


	////* 1�t���[�����Ƃɂ�����x��
	//rollingAmount += 0.01f;

	//=======================================�e�B�[�|�b�g============================================//

	//* ���s�ړ��̔z��
	Matrix transPot[20];

	for (int i = 0; i < 20; i++)
	{
		//* ��]
		//Matrix rotPotZ = Matrix::CreateRotationZ(0.0f);			/* ���l��0=�ω������Ȃ��Ă����A�Ȃ�Ȃ��Ă����炵�� */
		//Matrix rotPotX = Matrix::CreateRotationX(0.0f);
		Matrix rotPotY = Matrix::CreateRotationY(XMConvertToRadians(rand() % 360 + rollingAmount));
		Matrix rotPot = rotPotY;
		
		//* ���s�ړ�
		transPot[i] = Matrix::CreateTranslation(cosf(rand() % 360 + 1) * 50,
												0.0f, 
												sinf(rand() % 360 + 1) * 50);

		m_potWorld[i] = rotPot * transPot[i];

		//* 1�t���[�����Ƃɂ�����x��
		rollingAmount += 0.05f;
	}

	//=======================================���{�b�g�i���@�j============================================//

	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State g_key = keyboard->GetState();

	//* ���@�̃M�~�b�N
	
	/* �����ɁA�p�[�c���Ƃ̓����Ƃ������Ɨǂ��B�H���ē����������B */
	//
	//// ��
	//Vector3 angle = m_ObjPlayer1[ROBOT_PARTS_RWING].GetRotation();

	//m_ObjPlayer1[ROBOT_PARTS_RWING].SetRotation(
	//	angle += Vector3(X, Y, Z)
	//);
	// �T�C���p�����A�Ƃ������̂��g�������Bsinf�͉������������Ƃ��Ɏg�����́B
	// m_sinAngle++;
	// sinf(m_sinAngle)�Ƃ��ŉ����^���ɂȂ�Ƃ��Ȃ�Ƃ��B�����ʒu��Vector3�ő����Ă����邱��

	/* Rotation�Ȃ��]�BTraislation�Ȃ甭�ˁiSetParent��nullptr���Ȃ��Ƃ��Ă���j�BScale�Ȃ�g��k�� */



	// �X�y�[�X�L�[�����Ɣ�񂾂�Ă�����
	if (g_key.Space)
	{
		// �t���O��؂�ւ�
		m_isLanding = !m_isLanding;
	}

	if (m_isLanding)
	{
		m_ObjPlayer1[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 1.1f, 0));

		// ���łȂ��Ȃ猳�̈ʒu�ɖ߂�
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(0.5f, -0.3f, 0.85f));
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetScale(
			Vector3(1, 1, 1));

		m_ObjPlayer1[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-0.5f, -0.3f, 0.85f));
		m_ObjPlayer1[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		m_ObjPlayer1[ROBOT_PARTS_LWING].SetScale(
			Vector3(1, 1, 1));

	}
	if (!m_isLanding)
	{
		// ���ł���Ȃ�ʒu�Ɗp�x��ύX
		m_sinAngle += 0.05f;

		m_ObjPlayer1[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 2.1f + (sinf(m_sinAngle)/10), 0));
		
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(1.5f, 0.6f, 0.9f));
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		m_ObjPlayer1[ROBOT_PARTS_RWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));
		m_ObjPlayer1[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-1.5f, 0.6f, 0.9f));
		m_ObjPlayer1[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		m_ObjPlayer1[ROBOT_PARTS_LWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));

		m_camera->SetTargetPos(m_ObjPlayer1[ROBOT_PARTS_BODY].GetTranslation());
		m_camera->SetTargetAngle(m_ObjPlayer1[ROBOT_PARTS_BODY].GetRotation().y);

	}
	


	// A�L�[��������Ă�����
	if (g_key.A)
	{
		// ���@�̊p�x����]������
		//tank_angle += 0.03f;
		float angle = m_ObjPlayer1[0].GetRotation().y;
		m_ObjPlayer1[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// D�L�[��������Ă�����
	if (g_key.D)
	{
		// ���@�̊p�x����]������
		float angle = m_ObjPlayer1[0].GetRotation().y;
		m_ObjPlayer1[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// W�L�[��������Ă�����
	if (g_key.W)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��ʃx�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = m_ObjPlayer1[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�������
		Vector3 pos = m_ObjPlayer1[0].GetTranslation();
		m_ObjPlayer1[0].SetTranslation(pos + moveV);
	}

	// S�L�[��������Ă�����
	if (g_key.S)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, +0.1f);
		// �ړ��ʃx�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = m_ObjPlayer1[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�������
		Vector3 pos = m_ObjPlayer1[0].GetTranslation();
		m_ObjPlayer1[0].SetTranslation(pos + moveV);
	}

	//Keyboard::State g_key = keyboard->GetState();

	//// W�L�[�������ꂽ��O�i
	//if (g_key.W)
	//{
	//	// �ړ��ʂ̃x�N�g��(1�t���[�����Ƃ̑��x)
	//	Vector3 moveV(0.0f, 0.0f, -0.1f);

	//	//* �ړ��ʃx�N�g�����A���@�̊p�x�ɍ��킹��i�����Ă�����ɑO�i������j
	//	// ��]��������Ă���K�v������
	//	Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
	//	// TransformNormal�́A�x�N�g��(moveV)*�s��(rotRobot)�A�ŕ��s�ړ��𖳎�������́B
	//	// �܂���������̑傫�����A���������̊p�x�ňړ������邱�Ƃ��ł���B
	//	moveV = Vector3::TransformNormal(moveV, rotRobot);
	//	// �������̕��s�ړ��͖��������̂ŁA���̏ꍇrotRobot�̑����m_robotWorld�ł��B

	//	// ���@�̍��W���ړ�������
	//	robot_pos += moveV;



	//	//* Obj3D *//
	//	float angle = m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetRotation().y;

	//	Vector3 pos = m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetTranslation();
	//	m_ObjPlayer1[ROBOT_PARTS_CRAWLER].SetTranslation(pos + moveV);


	//}
	//// S�L�[�������ꂽ����
	//if (g_key.S)
	//{
	//	// �ړ��ʂ̃x�N�g��(1�t���[�����Ƃ̑��x)
	//	Vector3 moveV(0.0f, 0.0f, 0.1f);

	//	//* �ړ��ʃx�N�g�����A���@�̊p�x�ɍ��킹��i�����Ă�����ɑO�i������j
	//	Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
	//	// W�Ɠ�������
	//	moveV = Vector3::TransformNormal(moveV, rotRobot);

	//	// ���@�̍��W���ړ�������
	//	robot_pos += moveV;
	//}
	//// A�L�[�������ꂽ�獶����
	//if (g_key.A)
	//{
	//	//// ���@�̕����i���ʊp�j��ς���
	//	//Vector3 rote(0.0f, 0.1f, 0.0f);
	//	//// ���@�̊p�x��ς���
	//	//robot_angle += rote;

	//	//* Obj3D *//
	//	float angle = m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetRotation().y;
	//	m_ObjPlayer1[ROBOT_PARTS_CRAWLER].SetRotation(Vector3(0, angle + 0.03f, 0));

	//}
	//// D�L�[�������ꂽ��E����
	//if (g_key.D)
	//{
	//	//// ���@�̕����i���ʊp�j��ς���
	//	//Vector3 rote(0.0f, -0.1f, 0.0f);
	//	//// ���@�̊p�x��ς���
	//	//robot_angle += rote;

	//	//* Obj3D *//
	//	float angle = m_ObjPlayer1[ROBOT_PARTS_CRAWLER].GetRotation().y;
	//	m_ObjPlayer1[ROBOT_PARTS_CRAWLER].SetRotation(Vector3(0, angle - 0.03f, 0));
	//}

	//// ���[���h�s����v�Z
	////* �p�[�c�P�i�e�j
	//Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
	//Matrix transRobot = Matrix::CreateTranslation(robot_pos);
	//// ���W�ړ������l���Ă�̂łƂ肠�����Ԃ�����
	//m_robotWorld = rotRobot * transRobot;

	////* �p�[�c�Q�i�q�j
	//Matrix rotRobot2 = Matrix::CreateRotationZ(XM_PIDIV2/* ��/2 */) * Matrix::CreateRotationY(0);
	//Matrix transRobot2 = Matrix::CreateTranslation(Vector3(0, 0.5f, 0));
	//// ���W�ړ������l���Ă�̂łƂ肠�����Ԃ�����
	//m_robotWorld2 = rotRobot2 * transRobot2 * m_robotWorld;
	/* �����ʂ�X�P�[�����O�A��]�A�ړ��ʂ��|������ŁA
	�@ �p�[�c�P�Ɠ��������i���[���h�s��j���|�����킹�邱�ƂŁA
	�@ �p�[�c�P�ɒǏ]���������ɂł���B*/

	/* Obj3d�ł��ׂĂ�邱�Ƃ� */
	//m_ObjPlayer1[ROBOT_PARTS_CRAWLER].Draw();
	//m_ObjPlayer1[ROBOT_PARTS_HIP].Draw();
	//m_ObjPlayer1[ROBOT_PARTS_BODY].Draw();
	//m_ObjPlayer1[ROBOT_PARTS_HEAD].Draw();



	{	/* ���ƂŊ֐�������Ƃ����̂ŁA�Ƃ肠�����敪�����Ƃ� */

//		// �S���R�J�����ݒ�			/* ���@�̈ړ���������ɍs�� */
//		// ���@�ƃJ�����̋���
//		const float CAMERA_DISTANCE = 5.0f;
//		// ���_,�Q�Ɠ_
//		Vector3 eyepos, refpos;
//
//		// ���@��2m����Q�Ɠ_��
//		refpos = robot_pos + Vector3(0, 2.0f, 0);
//		// �Q�Ɠ_�Ǝ��_�̍����x�N�g��
//		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
//
//		// ���@�̌��ɉ�荞�ނ��߂̉�]�s��𐶐�
//		Matrix rotmat = Matrix::CreateRotationY(robot_angle.y);
		
//		// �����x�N�g������]������
//		cameraV = Vector3::TransformNormal(cameraV, rotmat);
//		// ���_���W���v�Z
//		eyepos = refpos + cameraV;

		// �n�ʂ̕`��H
		m_view = m_camera->GetView();
		m_proj = m_camera->GetProj();



	}



	//* vector�R���e�i��for��
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer1.begin();
		it != m_ObjPlayer1.end();
		it++)
	{
		it->Update();
	}

}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	// ���t���[���`�揈���͂����ɒǉ�����

	DirectX::CommonStates m_states(m_d3dDevice.Get());			/* �������܂Ń����o�ϐ��������̂�m_�ɂȂ��Ă��邪�C�ɂ��Ȃ��B�J�b�R���͏��������� */

	m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	m_d3dContext->RSSetState(m_states.CullNone());

	//// �x�[�V�b�N�G�t�F�N�g�Ɋe�s����Z�b�g
	//m_effect->SetWorld(m_world);								/* ���[���h�s�񂳂�����Ε\���͂����B���A�f�o�b�O�J�����g�p�̂��߂Ƀr���[�s��Ǝˉe�s����K�v */
	//m_effect->SetView(m_view);
	//m_effect->SetProjection(m_proj);

	//m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());


	// �n�ʃ��f���̕`��			/* cmo���f���̓v���~�e�B�u�o�b�`�s�v */
	m_groundModel->Draw(m_d3dContext.Get(), 
		m_states,
		Matrix::Identity, 
		m_view, 
		m_proj);

	//// �V�����f���̕`��
	//m_skydomeModel->Draw(m_d3dContext.Get(),
	//	m_states,
	//	Matrix::Identity,
	//	m_view,
	//	m_proj);

	// �V�����f��(Obj3d)�̕`��
	m_skydomeModel.Draw();


	//// �{�[�����f���̕`��i���K�j
	//m_PRACTICALballModel->Draw(m_d3dContext.Get(),
	//	m_states,
	//	m_PRACTICALballWorld,
	//	m_view,
	//	m_proj);

	//// �{�[�����\���ב�
	//for (int i = 0; i < 20; i++)
	//{
	//	m_ballModel->Draw(m_d3dContext.Get(),
	//		m_states,
	//		m_ballWorld[i],
	//		m_view,
	//		m_proj);
	//}

	//// �e�B�[�|�b�g����\���ב�
	//for (int i = 0; i < 20; i++)
	//{
	//	m_potModel->Draw(m_d3dContext.Get(),
	//		m_states,
	//		m_potWorld[i],
	//		m_view,
	//		m_proj);
	//}

	//// ���@���f���̕`��P
	//m_robotModel->Draw(m_d3dContext.Get(),
	//		m_states,
	//		m_robotWorld,
	//		m_view,
	//		m_proj);

	//// ���@���f���̕`��Q
	//m_robotModel->Draw(m_d3dContext.Get(),
	//	m_states,
	//	m_robotWorld2,
	//	m_view,
	//	m_proj);

	//* Obj3D�̎��@���f���̕`��
	for (std::vector<Obj3d>::iterator it = m_ObjPlayer1.begin();
		it != m_ObjPlayer1.end();
		it++)
	{
		it->Draw();
	}

	m_batch->Begin();

	//m_batch->DrawLine(
	//	VertexPositionColor(
	//		Vector3(0, 0, 0),
	//		Color(1, 1, 1)),
	//	VertexPositionColor(
	//		Vector3(800, 600, 0),
	//		Color(1, 1, 1))
	//);

	//// �O�p�`��`�悷�鏈��
	//VertexPositionColor v1(Vector3(0.5f, 0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->End();

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}