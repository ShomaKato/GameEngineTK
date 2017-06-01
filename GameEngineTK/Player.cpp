///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/01
//*
//*	@名前		Player.cpp
//*
//* @役割		プレイヤーのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// ヘッダのインクルード
#include "pch.h"
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


////----------------------------------------------------------------------
////! @関数名：Player
////!
////! @役割：プレイヤクラスのコンストラクタ
////!
////! @引数：なし(void)
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
Player::Player()
{

	// 自機パーツの読み込み
	player.resize(ROBOT_PARTS_NUM);
	player[ROBOT_PARTS_CRAWLER].LoadModel(L"Resources/robo_crawler.cmo");
	player[ROBOT_PARTS_HIP].LoadModel(L"Resources/robo_hip.cmo");
	player[ROBOT_PARTS_RWING].LoadModel(L"Resources/robo_wing.cmo");
	player[ROBOT_PARTS_LWING].LoadModel(L"Resources/robo_wing.cmo");
	player[ROBOT_PARTS_BODY].LoadModel(L"Resources/robo_body.cmo");
	player[ROBOT_PARTS_HEAD].LoadModel(L"Resources/robot.cmo");


	//* パーツの親子関係をセット
	// 腰部の親をキャタピラに
	player[ROBOT_PARTS_HIP].SetParent(
		&player[ROBOT_PARTS_CRAWLER]);
	// 胸部の親を腰部に
	player[ROBOT_PARTS_BODY].SetParent(
		&player[ROBOT_PARTS_HIP]);
	// 両翼の親を胸部に
	player[ROBOT_PARTS_RWING].SetParent(
		&player[ROBOT_PARTS_BODY]);
	player[ROBOT_PARTS_LWING].SetParent(
		&player[ROBOT_PARTS_BODY]);
	// 頭部の親を胸部に
	player[ROBOT_PARTS_HEAD].SetParent(
		&player[ROBOT_PARTS_BODY]);


	//* 親からのオフセット（座標のずらし分）をセット
	//* 腰
	// 座標調整
	player[ROBOT_PARTS_HIP].SetTranslation(
		Vector3(0, 0.4f, 0));
	// 角度調整
	player[ROBOT_PARTS_HIP].SetRotation(
		Vector3(0, 0, 0));
	// サイズ調整
	player[ROBOT_PARTS_HIP].SetScale(
		Vector3(1, 1, 1));

	//* 胸
	// 座標調整
	player[ROBOT_PARTS_BODY].SetTranslation(
		Vector3(0, 1.1f, 0));
	// 角度調整
	player[ROBOT_PARTS_BODY].SetRotation(
		Vector3(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f)));
	// サイズ調整
	player[ROBOT_PARTS_BODY].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	//* 右翼
	// 座標調整
	player[ROBOT_PARTS_RWING].SetTranslation(
		Vector3(0.5f, -0.3f, 0.85f));
	// 角度調整
	player[ROBOT_PARTS_RWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));

	//* 左翼
	// 座標調整
	player[ROBOT_PARTS_LWING].SetTranslation(
		Vector3(-0.5f, -.4f, 0.85f));
	// 角度調整	/* Z(右から右上)→Y(右から正面)→X(左から左下)の順で回転？ */ 展開はX
	player[ROBOT_PARTS_LWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));


	//* 頭
	// 座標調整
	player[ROBOT_PARTS_HEAD].SetTranslation(
		Vector3(0, 0.9f, 0));
	// 角度調整
	player[ROBOT_PARTS_HEAD].SetRotation(
		Vector3(0, 0, 0));
	// サイズ調整
	player[ROBOT_PARTS_HEAD].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	// キーボードを初期化する
	Keyboard = nullptr;

	// サイン用引数の初期化
	m_sinAngle = 0.0f;

}


////----------------------------------------------------------------------
////! @関数名：~Player
////!
////! @役割：プレイヤクラスのデストラクタ
////!
////! @引数：存在しない
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
Player::~Player()
{
}

////----------------------------------------------------------------------
////! @関数名：UpdatePlayer
////!
////! @役割：毎フレームの更新処理
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Player::UpdatePlayer()
{
	Keyboard::State key = Keyboard->GetState();


	// スペースキー押すと飛んだり墜ちたり
	if (key.Space)
	{
		// フラグを切り替え
		m_isLanding = !m_isLanding;
	}

	if (m_isLanding)
	{
		player[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 1.1f, 0));

		// 飛んでないなら元の位置に戻す
		player[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(0.5f, -0.3f, 0.85f));
		player[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		player[ROBOT_PARTS_RWING].SetScale(
			Vector3(1, 1, 1));

		player[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-0.5f, -0.3f, 0.85f));
		player[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		player[ROBOT_PARTS_LWING].SetScale(
			Vector3(1, 1, 1));

	}
	if (!m_isLanding)
	{
		// 飛んでいるなら位置と角度を変更
		m_sinAngle += 0.05f;

		player[ROBOT_PARTS_BODY].SetTranslation(
			Vector3(0, 2.1f + (sinf(m_sinAngle) / 10), 0));

		player[ROBOT_PARTS_RWING].SetTranslation(
			Vector3(1.5f, 0.6f, 0.9f));
		player[ROBOT_PARTS_RWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
		player[ROBOT_PARTS_RWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));
		player[ROBOT_PARTS_LWING].SetTranslation(
			Vector3(-1.5f, 0.6f, 0.9f));
		player[ROBOT_PARTS_LWING].SetRotation(
			Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
		player[ROBOT_PARTS_LWING].SetScale(
			Vector3(1.2f, 1.2f, 1.2f));
	}


	// Aキーが押されていたら
	if (key.A)
	{
		// 自機の角度を回転させる
		//tank_angle += 0.03f;
		float angle = player[0].GetRotation().y;
		player[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// Dキーが押されていたら
	if (key.D)
	{
		// 自機の角度を回転させる
		float angle = player[0].GetRotation().y;
		player[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// Wキーが押されていたら
	if (key.W)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動量ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = player[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動させる
		Vector3 pos = player[0].GetTranslation();
		player[0].SetTranslation(pos + moveV);
	}

	// Sキーが押されていたら
	if (key.S)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, +0.1f);
		// 移動量ベクトルを自機の角度分回転させる
		float angle = player[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動させる
		Vector3 pos = player[0].GetTranslation();
		player[0].SetTranslation(pos + moveV);
	}

	// vectorコンテナのfor文で、全パーツの更新処理を行う
	for (std::vector<Obj3d>::iterator it = player.begin();
		it != player.end();
		it++)
	{
		it->Update();
	}

}

////----------------------------------------------------------------------
////! @関数名：RenderPlayer
////!
////! @役割：オブジェクトを描画する
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Player::RenderPlayer()
{
	// vectorコンテナのfor文で、全パーツの描画処理を行う
	for (std::vector<Obj3d>::iterator it = player.begin();
		it != player.end();
		it++)
	{
		it->Draw();
	}
}

////----------------------------------------------------------------------
////! @関数名：SetKeyboard
////!
////! @役割：キーボードの情報を渡す関数
////!
////! @引数：入力されたキー情報(Keyboard* key)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Player::SetKeyboard(DirectX::Keyboard * key)
{
	Keyboard = key;
}

////----------------------------------------------------------------------
////! @関数名：GetPlayerPos
////!
////! @役割：参照点（プレイヤの位置）を受け取る関数
////!
////! @引数：なし(void)
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3 Player::GetPlayerTranslation()
{
	return player[0].GetTranslation();
}

////----------------------------------------------------------------------
////! @関数名：GetRotateY
////!
////! @役割：視点（カメラの位置）を受け取る関数
////!
////! @引数：なし(void)
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
float Player::GetPlayerRotationY()
{
	return player[0].GetRotation().y;
}
