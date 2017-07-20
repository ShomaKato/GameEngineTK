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



//* 休んでた分の先生の丸コピ
// 法線付き三角形（反時計回りが表面） 
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// 法線ベクトル 
};



// グローバル関数
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0,
					 const DirectX::SimpleMath::Vector3& _p1,
					 const DirectX::SimpleMath::Vector3& _p2, 
					 Triangle* _triangle);

bool CheckSphere2Triangle(const Sphere& _sphere,
						  const Triangle& _tri,
						  DirectX::SimpleMath::Vector3* _inter);

bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point,
						 const Triangle& _triangle);

bool CheckSegment2Triangle(const Segment& _segment,
	const Triangle& _triangle,
	DirectX::SimpleMath::Vector3 *_inter);