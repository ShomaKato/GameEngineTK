///-------------------------------------------------------------------
//*
//*	@作成日		2017/06/15
//*
//*	@名前		Collision.cpp
//*
//* @役割		衝突判定のクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// ヘッダのインクルード
#include "pch.h"
#include "Collision.h"


// 名前空間
using namespace DirectX::SimpleMath;


// グローバル関数
////----------------------------------------------------------------------
////! @関数名：CheckSphere2Sphere
////!
////! @役割：球と球の当たり判定を確認
////!
////! @引数：球A(Sphere)と球B(Sphere)
////!
////! @戻り値：ヒットしたか否か
////----------------------------------------------------------------------
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// 球Aと球Bの中心座標の差を計算する
	Vector3 sub = sphereA.Center - sphereB.Center;


	// 距離の二乗
	float distanceSquare;
	// 三平方の定理でベクトルの長さを計算する
	float distance = sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z);
	/* 差分ベクトルの長さは二点間の距離 */


	// 半径の和の二乗
	float radiusSquare;

	radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;


	// 距離が半径の和より大きければ当たっていない
	if (distance > sphereA.Radius + sphereB.Radius)
	{
		return false;
	}

	return true;
}


