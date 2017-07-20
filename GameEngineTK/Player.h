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
#include "CollisionNode.h"

class Player
{
	// パーツ一つ一つを格納するvector型コンテナ
	std::vector<Obj3d> player;

	// キーボードの情報
	DirectX::Keyboard* Keyboard;
	// 一回分だけ反応してくれるトリガー処理用キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker keyboardTracker;

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

	// 6/19 当たり判定モデルの読み込み・弾丸用の当たり判定
	SphereNode m_collisionNodeBullet;

public:
	Player();
	~Player();

	// 初期化関数
	void InitializePlayer();
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
	// 弾丸（任意のパーツ）を発射する関数
	void FireBullet();
	// 弾丸を再装填（パーツをリセット）する関数
	void ResetBullet();


	// 接地してるか否か
	bool m_isLanding = true;
	// sin用引数
	float m_sinAngle;


	// 弾を発射したか否か
	bool isEmpty;
	// 弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletV;
	// 弾丸発射秒数のカウント変数
	int cntFiring;


	// 6/19
	// 弾丸用当たり判定を取得
	const SphereNode& GetCollisionNodeBullet() { return m_collisionNodeBullet; }
	// 当たり判定の表示非表示フラグ
	bool isCollisionVisible;


	// 7/10　プレイヤにも全身当たり判定を
	// 全身の当たり判定
	SphereNode m_collisionNode;
	// 当たり判定(とりあえず球型)を取得
	const SphereNode& GetCollisionNode() { return m_collisionNode; }
};