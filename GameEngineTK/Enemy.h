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

public:
	Enemy();
	~Enemy();

	// 初期化関数
	void InitializeEnemy();
	// 更新関数
	void UpdateEnemy();
	// 描画関数
	void RenderEnemy();

	// 接地してるか否か
	bool m_isLanding = true;
	// sin用引数
	float m_sinAngle;


	// タイマー
	int m_timer;
	// 角度の変更
	float m_DistAngle;
};