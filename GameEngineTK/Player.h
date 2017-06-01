///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/01
//*
//*	@名前		Player.h
//*
//* @役割		プレイヤーのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------
// 多重インクルードの防止
#pragma once

// ヘッダのインクルード
#include <Keyboard.h>
#include "Obj3d.h"

class Player
{
	// パーツ一つ一つを格納するvector型コンテナ
	std::vector<Obj3d> player;

	// キーボードの情報
	DirectX::Keyboard* Keyboard;

	// 自機パーツ
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
	Player();
	~Player();

	// 更新関数
	void UpdatePlayer();
	// 描画関数
	void RenderPlayer();

	// キーボード情報を渡す関数
	void SetKeyboard(DirectX::Keyboard* key);
	// 参照点を受け取る関数
	const DirectX::SimpleMath::Vector3 GetPlayerTranslation();
	// 視点を受け取る関数
	float GetPlayerRotationY();


	// 接地してるか否か
	bool m_isLanding = true;
	// sin用引数
	float m_sinAngle;
};