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

	// 初期化はここに追加する
	// プリミティブバッチの作成
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());					/* unique_ptrはスマートポインタの一種。deleteを自動でやってくれる */		
																					/* Get()は、ユニークポインタを普通のポインタに変換するための関数 */

	// ワールド行列の初期化？		/* 追記：Identifyは単位行列。まあつまり初期化でおｋ */
	m_world = Matrix::Identity;

	// ビュー行列と射影行列の設定
	m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight),
		0.1f,	
		500.f);


	// ベーシックエフェクトの作成・初期化					
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

	// プリミティブバッチの作成・初期化
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());

	// デバッグカメラの作成
	m_debugCamera = make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// エフェクトファクトリを生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャの読み込みパス指定		/* ここからテクスチャを読み込みますよ、という意味に */
	m_factory->SetDirectory(L"Resources");

	// 地面モデルの読み込み
	m_groundModel = Model::CreateFromCMO(m_d3dDevice.Get(), 
									L"Resources\\ground200m.cmo", 
									*m_factory);	
	// 天球モデルの読み込み
	m_skydomeModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		L"Resources\\SkyDome.cmo",
		*m_factory);

	// 球並べるやつ・20回読み込む
	m_ballModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		(L"Resources\\ball.cmo"),
		*m_factory);

	// ティーポット並べるやつ・20回読み込む
	m_potModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		(L"Resources\\edfPot.cmo"),
		*m_factory);

	// ロボモデルの読み込み
	m_robotModel = Model::CreateFromCMO(m_d3dDevice.Get(),
		(L"Resources\\robot.cmo"),
		*m_factory);

	rollingAmount = 0.0f;

	// キーボードの初期化
	keyboard = make_unique<Keyboard>();



	// カメラの初期化
	m_camera = make_unique<FollowCamera>(m_outputWidth,m_outputHeight);

	//* カメラにキーボードを渡す
	m_camera->SetKeyboard(keyboard.get());

	//=====ここまでで、初期化設定は完了=====//
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

	// 毎フレーム更新処理はここに追加する

	// デバックガメラを毎フレーム更新
	m_debugCamera->Update();
	// ビュー行列を取得
	//m_view = m_debugCamera->GetCameraMatrix();


	// 自作カメラを毎フレーム更新し、ビュー行列,射影行列を受け取る
	//m_camera->Update();
	//m_view = m_camera->GetView();
	//m_proj = m_camera->GetProj();
	//// 自機にカメラがついてくる
	//m_camera->SetEyepos(robot_pos);
	//m_camera->SetRefpos(Vector3(0, 0, -100.0f));		/* eyeposとrefposが同じになるのでずらす */

	/* 上記はゴム紐カメラが完成したので用済みである */

	//=====================球のワールド行列を計算（アップデートでやる）==========================//

	//* スケーリング		/* (倍率) */
	Matrix scalemat = Matrix::CreateScale(0.1f);

	//* 回転				/* Z,X,Yの順番で一個ずつでなければダメな上にラジアン */
	// 旋回
	Matrix rotmatZ = Matrix::CreateRotationZ(XM_PIDIV4);							/* 1/4π */
	// 仰角
	Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(15.0f));			/* 度数への変換関数 */
	// 方位角
	Matrix rotmatY = Matrix::CreateRotationY(XM_1DIVPI);							/* ちなみにDIVの方が細かく計算できるらしい */
	// 回転行列の合成
	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//* 平行移動	/* (X軸,Y軸,Z軸) */
	Matrix transmat = Matrix::CreateTranslation(1.0f, 2.0f, 0.0f);


	// ワールド行列の合成			/* 必ずSRTの順で掛け合わせる */
	m_PRACTICALballWorld = scalemat * rotmat * transmat;


	//===========================================================================================//

	//=======================================上の本番============================================//

	//* 平行移動	/* こっちを先に掛けて短縮化を図る */
	Matrix transmats1 = Matrix::CreateTranslation(20.0f, 0.0f, 0.0f);			/* 冷静に考えたら平行移動の行列は二個でよかったので、for文の外に */
	Matrix transmats2 = Matrix::CreateTranslation(40.0f, 0.0f, 0.0f);
	Matrix rotmats[20];

	for (int i = 0; i < 10; i++)
	{
		// 回転
		Matrix rotmatsZ1 = Matrix::CreateRotationZ(0.0f);
		Matrix rotmatsX1 = Matrix::CreateRotationX(0.0f);
		Matrix rotmatsY1 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i));
		Matrix rotmatsY2 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i));

	
			rotmatsY1 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i) + rollingAmount);
		
			rotmatsY2 = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i) - rollingAmount);
		

		rotmats[i] = rotmatsZ1 * rotmatsX1 * rotmatsY1;

		// ワールド行列の合成
		m_ballWorld[i] = transmats1 * rotmats[i];
		// 10～19は外側
		rotmats[i +10] = rotmatsY2;
		m_ballWorld[i + 10] = transmats2 * rotmats[i + 10];
	}

	////* 1フレームごとにある程度回す
	//rollingAmount += 0.01f;

	//=======================================ティーポット============================================//

	//* 平行移動の配列
	Matrix transPot[20];

	for (int i = 0; i < 20; i++)
	{
		//* 回転
		//Matrix rotPotZ = Matrix::CreateRotationZ(0.0f);			/* 数値が0=変化させなくていい、ならなくていいらしい */
		//Matrix rotPotX = Matrix::CreateRotationX(0.0f);
		Matrix rotPotY = Matrix::CreateRotationY(XMConvertToRadians(rand() % 360 + rollingAmount));
		Matrix rotPot = rotPotY;
		
		//* 平行移動
		transPot[i] = Matrix::CreateTranslation(cosf(rand() % 360 + 1) * 50,
												0.0f, 
												sinf(rand() % 360 + 1) * 50);

		m_potWorld[i] = rotPot * transPot[i];

		//* 1フレームごとにある程度回す
		rollingAmount += 0.05f;
	}

	//=======================================ロボット============================================//

	Keyboard::State g_key = keyboard->GetState();

	// Wキーが押されたら前進
	if (g_key.W)
	{
		// 移動量のベクトル(1フレームごとの速度)
		Vector3 moveV(0.0f, 0.0f, -0.1f);

		//* 移動量ベクトルを、自機の角度に合わせる（向いてる方向に前進させる）
		// 回転具合を持ってくる必要がある
		Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
		// TransformNormalは、ベクトル(moveV)*行列(rotRobot)、で平行移動を無視するもの。
		// つまり第一引数分の大きさを、第二引数分の角度で移動させることができる。
		moveV = Vector3::TransformNormal(moveV, rotRobot);
		// 第二引数の平行移動は無視されるので、この場合rotRobotの代わりにm_robotWorldでも可。

		// 自機の座標を移動させる
		robot_pos += moveV;
	}
	// Sキーが押されたら後退
	if (g_key.S)
	{
		// 移動量のベクトル(1フレームごとの速度)
		Vector3 moveV(0.0f, 0.0f, 0.1f);

		//* 移動量ベクトルを、自機の角度に合わせる（向いてる方向に前進させる）
		Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
		// Wと同じ処理
		moveV = Vector3::TransformNormal(moveV, rotRobot);

		// 自機の座標を移動させる
		robot_pos += moveV;
	}
	// Aキーが押されたら左旋回
	if (g_key.A)
	{
		// 自機の方向（方位角）を変える
		Vector3 rote(0.0f, 0.1f, 0.0f);
		// 自機の角度を変える
		robot_angle += rote;
	}
	// Dキーが押されたら右旋回
	if (g_key.D)
	{
		// 自機の方向（方位角）を変える
		Vector3 rote(0.0f, -0.1f, 0.0f);
		// 自機の角度を変える
		robot_angle += rote;
	}

	// ワールド行列を計算
	Matrix transRobot = Matrix::CreateTranslation(robot_pos);
	Matrix rotRobot = Matrix::CreateRotationY(robot_angle.y);
	// 座標移動だけ考えてるのでとりあえずぶち込む
	m_robotWorld = rotRobot * transRobot;


	{	/* あとで関数化するといいので、とりあえず区分けしとく */

//		// ゴム紐カメラ設定			/* 自機の移動処理より後に行う */
//		// 自機とカメラの距離
//		const float CAMERA_DISTANCE = 5.0f;
//		// 視点,参照点
//		Vector3 eyepos, refpos;
//
//		// 自機の2m上を参照点に
//		refpos = robot_pos + Vector3(0, 2.0f, 0);
//		// 参照点と視点の差分ベクトル
//		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
//
//		// 自機の後ろに回り込むための回転行列を生成
//		Matrix rotmat = Matrix::CreateRotationY(robot_angle.y);
		
//		// 差分ベクトルを回転させる
//		cameraV = Vector3::TransformNormal(cameraV, rotmat);
//		// 視点座標を計算
//		eyepos = refpos + cameraV;

		m_camera->SetTargetPos(robot_pos);
		m_camera->SetTargetAngle(robot_angle.y);

		m_camera->Update();	
		m_view = m_camera->GetView();
		m_proj = m_camera->GetProj();

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
	// 毎フレーム描画処理はここに追加する

	DirectX::CommonStates m_states(m_d3dDevice.Get());			/* さっきまでメンバ変数だったのでm_になっているが気にしない。カッコ内は初期化処理 */

	m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	m_d3dContext->RSSetState(m_states.CullNone());

	// ベーシックエフェクトに各行列をセット
	m_effect->SetWorld(m_world);								/* ワールド行列さえあれば表示はされる。が、デバッグカメラ使用のためにビュー行列と射影行列も必要 */
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());


	// 地面モデルの描画			/* cmoモデルはプリミティブバッチ不要 */
	m_groundModel->Draw(m_d3dContext.Get(), 
		m_states,
		Matrix::Identity, 
		m_view, 
		m_proj);

	// 天球モデルの描画
	m_skydomeModel->Draw(m_d3dContext.Get(),
		m_states,
		m_world,
		m_view,
		m_proj);

	//// ボールモデルの描画（練習）
	//m_PRACTICALballModel->Draw(m_d3dContext.Get(),
	//	m_states,
	//	m_PRACTICALballWorld,
	//	m_view,
	//	m_proj);

	//// ボールを二十個並べ隊
	//for (int i = 0; i < 20; i++)
	//{
	//	m_ballModel->Draw(m_d3dContext.Get(),
	//		m_states,
	//		m_ballWorld[i],
	//		m_view,
	//		m_proj);
	//}

	//// ティーポットも二十個並べ隊
	//for (int i = 0; i < 20; i++)
	//{
	//	m_potModel->Draw(m_d3dContext.Get(),
	//		m_states,
	//		m_potWorld[i],
	//		m_view,
	//		m_proj);
	//}

	// ロボットモデルの描画
	m_robotModel->Draw(m_d3dContext.Get(),
			m_states,
			m_robotWorld,
			m_view,
			m_proj);


	m_batch->Begin();

	//m_batch->DrawLine(
	//	VertexPositionColor(
	//		Vector3(0, 0, 0),
	//		Color(1, 1, 1)),
	//	VertexPositionColor(
	//		Vector3(800, 600, 0),
	//		Color(1, 1, 1))
	//);

	// 三角形を描画する処理
	VertexPositionColor v1(Vector3(0.5f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	m_batch->DrawTriangle(v1, v2, v3);

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