///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/15
//*
//*	@名前		Collision.h
//*
//* @役割		衝突判定のクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// 多重インクルードの防止
#pragma once

// ヘッダーのインクルード
#include "Collision.h"
#include "Obj3d.h"

// 当たり判定の親ノード
class CollisionNode
{
public:		/* 当たり判定の共通機能はここに */
	// 初期化
	virtual void Initialize() = 0;	/* virtualつけて=0したら純粋仮想関数。中身は継承先に任せる */
	// 毎フレーム更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;

	// 親ノード(Objd型)を設定
	void SetParent(Obj3d * parent);

	// 親からのオフセット（座標のズレ）を設定
	void SetTrans(const DirectX::SimpleMath::Vector3 trans) { m_Trans = trans; }

protected:
	// デバッグ表示用オブジェクト（当たり判定が見えるように）
	Obj3d m_obj;
	// 親からのオフセット（座標のズレ）
	DirectX::SimpleMath::Vector3 m_Trans;

};


// 球の当たり判定ノード
class SphereNode : public CollisionNode, public Sphere
{
public:
	// コンストラクタ
	SphereNode();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();

	// 拡縮前の半径をセット
	void SetLocalRadius(const float radius) { m_localRadius = radius; }
	// 拡縮前の半径をセット

protected:
	// スケーリングで拡縮される前の当たり判定の半径
	float m_localRadius;

};