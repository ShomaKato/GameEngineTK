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
#include <Windows.h>		/* 何か、これつけとくとエラー消えることとかあるらしい */
#include <d3d11_1.h>
#include <SimpleMath.h>


//namespace Collision
//* 本来は↑のように、ライブラリ作っておく。授業だとキリがないので割愛




// 球状の衝突判定クラス
class Sphere
{
public:			/* データバンクとして使うだけで、関数とかを持たせるつもりはないのでパブリック */
	// 中心座標
	DirectX::SimpleMath::Vector3 Center;
	// 半径
	float Radius;
	// コンストラクタ
	Sphere()
	{
		// デフォルトで半径一メートル
		Radius = 1.0f;		/* この初期化を行いたいがためにクラス化。本来ならストラクトでおｋ */
	}
};

// 線分(Sphereと組み合わせてカプセルにする)
class Segment
{
public:
	// 始点
	DirectX::SimpleMath::Vector3 Start;
	// 終点
	DirectX::SimpleMath::Vector3 End;



};


// グローバル関数
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB);