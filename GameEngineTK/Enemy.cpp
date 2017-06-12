///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/05
//*
//*	@名前		Enemy.cpp
//*
//* @役割		エネミーのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// ヘッダのインクルード
#include "pch.h"
#include "enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


////----------------------------------------------------------------------
////! @関数名：Enemy
////!
////! @役割：プレイヤクラスのコンストラクタ
////!
////! @引数：なし(void)
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
Enemy::Enemy()
{
	// 初期化関数
	InitializeEnemy();
}


////----------------------------------------------------------------------
////! @関数名：~Enemy
////!
////! @役割：プレイヤクラスのデストラクタ
////!
////! @引数：存在しない
////!
////! @戻り値：存在しない
////----------------------------------------------------------------------
Enemy::~Enemy()
{
}

////----------------------------------------------------------------------
////! @関数名：InitializeEnemy
////!
////! @役割：エネミーオブジェクトの初期化
////!
////! @引数：ない（void）
////!
////! @戻り値：ない（void）
////----------------------------------------------------------------------
void Enemy::InitializeEnemy()
{

	// 自機パーツの読み込み
	enemy.resize(ROBOT_PARTS_NUM);
	enemy[ROBOT_PARTS_CRAWLER].LoadModel(L"Resources/robo_crawler.cmo");
	enemy[ROBOT_PARTS_HIP].LoadModel(L"Resources/robo_hip.cmo");
	enemy[ROBOT_PARTS_RWING].LoadModel(L"Resources/robo_wing.cmo");
	enemy[ROBOT_PARTS_LWING].LoadModel(L"Resources/robo_wing.cmo");
	enemy[ROBOT_PARTS_BODY].LoadModel(L"Resources/robo_body.cmo");
	enemy[ROBOT_PARTS_HEAD].LoadModel(L"Resources/robot.cmo");


	//* パーツの親子関係をセット
	// 腰部の親をキャタピラに
	enemy[ROBOT_PARTS_HIP].SetParent(
		&enemy[ROBOT_PARTS_CRAWLER]);
	// 胸部の親を腰部に
	enemy[ROBOT_PARTS_BODY].SetParent(
		&enemy[ROBOT_PARTS_HIP]);
	// 両翼の親を胸部に
	enemy[ROBOT_PARTS_RWING].SetParent(
		&enemy[ROBOT_PARTS_BODY]);
	enemy[ROBOT_PARTS_LWING].SetParent(
		&enemy[ROBOT_PARTS_BODY]);
	// 頭部の親を胸部に
	enemy[ROBOT_PARTS_HEAD].SetParent(
		&enemy[ROBOT_PARTS_BODY]);


	//* 親からのオフセット（座標のずらし分）をセット
	//* 腰
	// 座標調整
	enemy[ROBOT_PARTS_HIP].SetTranslation(
		Vector3(0, 0.4f, 0));
	// 角度調整
	enemy[ROBOT_PARTS_HIP].SetRotation(
		Vector3(0, 0, 0));
	// サイズ調整
	enemy[ROBOT_PARTS_HIP].SetScale(
		Vector3(1, 1, 1));

	//* 胸
	// 座標調整
	enemy[ROBOT_PARTS_BODY].SetTranslation(
		Vector3(0, 1.1f, 0));
	// 角度調整
	enemy[ROBOT_PARTS_BODY].SetRotation(
		Vector3(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(0.0f)));
	// サイズ調整
	enemy[ROBOT_PARTS_BODY].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	//* 右翼
	// 座標調整
	enemy[ROBOT_PARTS_RWING].SetTranslation(
		Vector3(0.5f, -0.3f, 0.85f));
	// 角度調整
	enemy[ROBOT_PARTS_RWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));

	//* 左翼
	// 座標調整
	enemy[ROBOT_PARTS_LWING].SetTranslation(
		Vector3(-0.5f, -.4f, 0.85f));
	// 角度調整	/* Z(右から右上)→Y(右から正面)→X(左から左下)の順で回転？ */ 展開はX
	enemy[ROBOT_PARTS_LWING].SetRotation(
		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));


	//* 頭
	// 座標調整
	enemy[ROBOT_PARTS_HEAD].SetTranslation(
		Vector3(0, 0.9f, 0));
	// 角度調整
	enemy[ROBOT_PARTS_HEAD].SetRotation(
		Vector3(0, 0, 0));
	// サイズ調整
	enemy[ROBOT_PARTS_HEAD].SetScale(
		Vector3(0.85f, 0.85f, 0.85f));

	// サイン用引数の初期化
	m_sinAngle = 0.0f;


	// 初期配置(超適当ランダム)
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	// 一秒タイマーの初期化
	m_timer = 60;

	// 目標角度の初期化
	m_DistAngle = 0.0f;

}

////----------------------------------------------------------------------
////! @関数名：UpdateEnemy
////!
////! @役割：毎フレームの更新処理
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Enemy::UpdateEnemy()
{

	//// スペースキー押すと飛んだり墜ちたり
	//if (key.Space)
	//{
	//	// フラグを切り替え
	//	m_isLanding = !m_isLanding;
	//}

	//if (m_isLanding)
	//{
	//	// 飛んでないなら元の位置に戻す
	//	enemy[ROBOT_PARTS_BODY].SetTranslation(
	//		Vector3(0, 1.1f, 0));

	//	enemy[ROBOT_PARTS_RWING].SetTranslation(
	//		Vector3(0.5f, -0.3f, 0.85f));
	//	enemy[ROBOT_PARTS_RWING].SetRotation(
	//		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
	//	enemy[ROBOT_PARTS_RWING].SetScale(
	//		Vector3(1, 1, 1));

	//	enemy[ROBOT_PARTS_LWING].SetTranslation(
	//		Vector3(-0.5f, -0.3f, 0.85f));
	//	enemy[ROBOT_PARTS_LWING].SetRotation(
	//		Vector3(XMConvertToRadians(-30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
	//	enemy[ROBOT_PARTS_LWING].SetScale(
	//		Vector3(1, 1, 1));

	//}
	//if (!m_isLanding)
	//{
	//	// 飛んでいるなら位置と角度を変更
	//	m_sinAngle += 0.05f;

	//	enemy[ROBOT_PARTS_BODY].SetTranslation(
	//		Vector3(0, 2.1f + (sinf(m_sinAngle) / 10), 0));

	//	enemy[ROBOT_PARTS_RWING].SetTranslation(
	//		Vector3(1.5f, 0.6f, 0.9f));
	//	enemy[ROBOT_PARTS_RWING].SetRotation(
	//		Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(-100.0f), XMConvertToRadians(110.0f)));
	//	enemy[ROBOT_PARTS_RWING].SetScale(
	//		Vector3(1.2f, 1.2f, 1.2f));
	//	enemy[ROBOT_PARTS_LWING].SetTranslation(
	//		Vector3(-1.5f, 0.6f, 0.9f));
	//	enemy[ROBOT_PARTS_LWING].SetRotation(
	//		Vector3(XMConvertToRadians(30.0f), XMConvertToRadians(100.0f), XMConvertToRadians(70.0f)));
	//	enemy[ROBOT_PARTS_LWING].SetScale(
	//		Vector3(1.2f, 1.2f, 1.2f));
	//}


	//// Aキーが押されていたら
	//if (key.A)
	//{
	//	// 自機の角度を回転させる
	//	//tank_angle += 0.03f;
	//	float angle = enemy[0].GetRotation().y;
	//	enemy[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	//}

	//// Dキーが押されていたら
	//if (key.D)
	//{
	//	// 自機の角度を回転させる
	//	float angle = enemy[0].GetRotation().y;
	//	enemy[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	//}

	//// Wキーが押されていたら
	//if (key.W)
	//{
	//	// 移動量のベクトル
	//	Vector3 moveV(0, 0, -0.1f);
	//	// 移動量ベクトルを自機の角度分回転させる
	//	//moveV = Vector3::TransformNormal(moveV, tank_world);
	//	float angle = enemy[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//	// 自機の座標を移動させる
	//	Vector3 pos = enemy[0].GetTranslation();
	//	enemy[0].SetTranslation(pos + moveV);
	//}

	//// Sキーが押されていたら
	//if (key.S)
	//{
	//	// 移動量のベクトル
	//	Vector3 moveV(0, 0, +0.1f);
	//	// 移動量ベクトルを自機の角度分回転させる
	//	float angle = enemy[0].GetRotation().y;
	//	Matrix rotmat = Matrix::CreateRotationY(angle);
	//	moveV = Vector3::TransformNormal(moveV, rotmat);
	//	// 自機の座標を移動させる
	//	Vector3 pos = enemy[0].GetTranslation();
	//	enemy[0].SetTranslation(pos + moveV);
	//}



	// AI
	m_timer--;
	if (m_timer < 0)
	{
		// カウントが0に達したら
		m_timer = 60;

		// 目標角度を変更
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		
	}




	// vectorコンテナのfor文で、全パーツの更新処理を行う
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Update();
	}

}

////----------------------------------------------------------------------
////! @関数名：RenderEnemy
////!
////! @役割：オブジェクトを描画する
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Enemy::RenderEnemy()
{
	// vectorコンテナのfor文で、全パーツの描画処理を行う
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Draw();
	}
}

