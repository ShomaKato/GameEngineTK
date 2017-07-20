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

	this->SetTrans(pos);

	// 一秒タイマーの初期化
	m_Timer = 60;

	// 目標角度の初期化
	m_DistAngle = 0.0f;



	//// 6/19
	//{
	//	// 当たり判定を設置
	//	m_collisionNode.Initialize();
	//	// 当たり判定の表示非表示フラグをOFF
		isCollisionVisible = false;
	//}
	m_collisionNode.Initialize();
	m_collisionNode.SetParent(&enemy[ROBOT_PARTS_BODY]);
	//* 上のSetParentはupdateでのみ行っていたが、初期化でもやらないと当たり判定が初期原点に来てしまう。
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
	// 押されているキーを検索
	Keyboard::State key = Keyboard->GetState();
	// キーボードの状態取得（トリガー用）
	Keyboard::State keyboardstate = Keyboard->GetState();
	keyboardTracker.Update(keyboardstate);

	// AI		/* 先生の丸パクリなので、よく研究しておくこと */
	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;
		// -0.5～+0.5の乱数
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;

		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}

	{
		// 角度を回転させる
		Vector3 rotv = enemy[0].GetRotation();

		float angle = m_DistAngle - rotv.y;

		// 180度を超えていた場合、逆回りの方が近い
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}

		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// 補間
		rotv.y += angle * 0.05f;

		SetRot(rotv);
	}

	// 機体が向いている方向に進む
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.05f);
		// 移動量ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, tank_world);
		float angle = enemy[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動させる
		Vector3 pos = enemy[0].GetTranslation();
		enemy[0].SetTranslation(pos + moveV);
	}


	// 6/19
	if (keyboardTracker.IsKeyPressed(Keyboard::Keys::O))
	{
		// Oキー押したら当たり判定の表示非表示フラグをOFF
		isCollisionVisible = !isCollisionVisible;
	}


	Calc();

}

////----------------------------------------------------------------------
////! @関数名：Calc
////!
////! @役割：オブジェクトを描画する
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Enemy::Calc()
{
	// vectorコンテナのfor文で、全パーツの更新処理を行う
	for (std::vector<Obj3d>::iterator it = enemy.begin();
		it != enemy.end();
		it++)
	{
		it->Update();
	}

	// 6/19
	{
		// 当たり判定を設置
		m_collisionNode.Update();
		// 当たり判定を弾丸モデルに親子付
		m_collisionNode.SetParent(&enemy[ROBOT_PARTS_BODY]);
		m_collisionNode.SetTrans(Vector3(0, 0.5f, 0));		/* 好きな位置に直す */
		m_collisionNode.SetLocalRadius(1.5f);				/* 好きな大きさに直す */
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


	// 6/19
	if (isCollisionVisible)
	{
		// 弾丸用の当たり判定を設置
		m_collisionNode.Draw();
	}
}


const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// タンクパーツの座標を返す
	return enemy[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// タンクパーツの回転を返す
	return enemy[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	enemy[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	enemy[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return enemy[0].GetWorld();
}







////----------------------------------------------------------------------
////! @関数名：SetKeyboard
////!
////! @役割：キーボードを渡す関数
////!
////! @引数：入力されたキー情報(Keyboard* key)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Enemy::SetKeyboard(DirectX::Keyboard * key)
{
	Keyboard = key;
}
