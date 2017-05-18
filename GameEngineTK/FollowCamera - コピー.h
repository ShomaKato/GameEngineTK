///-------------------------------------------------------------------
//*
//*	@作成日		2017/05/15
//*
//*	@名前		FollowCamera.h
//*
//*	@著者		加藤 彰馬
//*
//* @役割		自機に追従するカメラクラス
//*
///-------------------------------------------------------------------

// 多重インクルードの防止
#pragma once

// ヘッダファイルのインクルード
#include "Camera.h"

class FollowCamera :public Camera
{
public:
	// 静的な変数の定義
	//static const float FollowCamera::CAMERA_DISTANCE;	/* FollowCameraのコレ、という指定に */

	FollowCamera(int width, int height);
	void Update() override;				/* 仮想関数にのみ使える。こっちはこっちの処理やで、という意味に */

	// 視点をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos);

	// 追従対象の角度をセット
	void SetTargetAngle(const float& targetAngle);

protected:		/* おそらくこれを継承することはないので、privateでもまあおｋ、とのこと */
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の回転角
	float m_targetAngle;
	

};

