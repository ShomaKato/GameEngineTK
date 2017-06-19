///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/05
//*
//*	@名前		Enemy.h
//*
//* @役割		エネミーのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------
// 多重インクルードの防止
#pragma once

// ヘッダのインクルード
#include "Obj3d.h"
#include "CollisionNode.h"

class Enemy
{
	// パーツ一つ一つを格納するvector型コンテナ
	std::vector<Obj3d> enemy;

	// パーツ		/* 親であればあるほど先に */
	enum ROBOT_PARTS
	{
		ROBOT_PARTS_CRAWLER,
		ROBOT_PARTS_HIP,
		ROBOT_PARTS_RWING,
		ROBOT_PARTS_LWING,
		ROBOT_PARTS_BODY,
		ROBOT_PARTS_HEAD,

		ROBOT_PARTS_NUM
	};


	// 6/19 当たり判定モデルの読み込み・弾丸用の当たり判定
	SphereNode m_collisionNode;

public:
	Enemy();
	~Enemy();

	// 初期化関数
	void InitializeEnemy();
	// 更新関数
	void UpdateEnemy();
	// 行列更新
	void Calc();
	// 描画関数
	void RenderEnemy();


	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 回転を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// 回転を設定
	void SetRot(const DirectX::SimpleMath::Vector3& rot);

	/* SetTransとSetRotの引数のconstを消したらエラーが消えた */
	/* ↑その二つは継承関数。継承元にconstがついてなかったためエラーだった */

	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();


	// 接地してるか否か
	bool m_isLanding = true;
	// sin用引数
	float m_sinAngle;


	// タイマー
	int m_Timer;
	// 角度の変更
	float m_DistAngle;


	// 6/19
	// 当たり判定(とりあえず球型)を取得
	const SphereNode& GetCollisionNodeBullet() { return m_collisionNode; }
	// 当たり判定の表示非表示フラグ
	bool isCollisionVisible;
};