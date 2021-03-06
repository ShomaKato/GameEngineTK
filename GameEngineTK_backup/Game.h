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


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	enum ROBOT_PARTS		/* 親になるパーツほど先に書くように */
	{
		ROBOT_PARTS_CRAWLER,
		ROBOT_PARTS_HIP,
		ROBOT_PARTS_RWING,
		ROBOT_PARTS_LWING,
		ROBOT_PARTS_BODY,
		ROBOT_PARTS_HEAD,

		ROBOT_PARTS_NUM		/* ここに置いとけば、これがパーツ数の総数に */
	};

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


	//* cppでグローバル変数にしていたものを移動してメンバ変数に
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	////* 描画用行列のメンバ変数
	//DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	////* デバッグカメラ
	//std::unique_ptr<DebugCamera> m_debugCamera;


	//* エフェクトファクトリ
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//* モデル			/* モデルの容れ物 */
	std::unique_ptr<DirectX::Model> m_groundModel;
	//std::unique_ptr<DirectX::Model> m_skydomeModel;
	std::unique_ptr<DirectX::Model> m_PRACTICALballModel;
	std::unique_ptr<DirectX::Model> m_ballModel;
	std::unique_ptr<DirectX::Model> m_robotModel;

	// 手始めに天球を、作ったクラスの型にしてみる
	Obj3d m_skydomeModel;


	std::unique_ptr<DirectX::Model> m_potModel;

	//* 球のワールド行列（練習）
	DirectX::SimpleMath::Matrix m_PRACTICALballWorld;
	//* 球のワールド行列（本番）
	DirectX::SimpleMath::Matrix m_ballWorld[20];
	//* ティーポットのワールド行列
	DirectX::SimpleMath::Matrix m_potWorld[20];


	//* 天球ズの回転量
	float rollingAmount;

	//* キーボードの登録的なサムシング
	std::unique_ptr<DirectX::Keyboard> keyboard;

	//* 自機（ロボット）の座標
	DirectX::SimpleMath::Vector3 robot_pos;
	//* 自機の回転角
	DirectX::SimpleMath::Vector3 robot_angle;
	////* 自機のワールド行列１
	//DirectX::SimpleMath::Matrix m_robotWorld;	
	////* 自機のワールド行列２
	//DirectX::SimpleMath::Matrix m_robotWorld2;

	// プレイヤの3Dオブジェクト
	std::vector<Obj3d> m_ObjPlayer1;		/* vectorコンテナ。パーツモデルをひとまとめにできる */
	Obj3d m_ObjPlayer2;

	// サイン用の引数となる角度
	float m_sinAngle;

	// 作ったカメラ
	std::unique_ptr<FollowCamera> m_camera;






	// 接地してるか否か
	bool m_isLanding = true;


	//// デバッグ文字表示用スプライドバッチ
	//std::unique_ptr<SpriteBatch> m_spriteBatch;

	//// スプライトフォント
	//std::unique_ptr<SpriteFont> m_spriteFont;

	//// コモンステート
	//std::unique_ptr<CommonStates> m_state;


};