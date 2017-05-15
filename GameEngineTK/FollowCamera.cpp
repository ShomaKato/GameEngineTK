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

// ヘッダファイルのインクルード
#include "pch.h"
#include "FollowCamera.h"

// 名前空間
using namespace DirectX::SimpleMath;

// 静的な云々かんぬん
//const float CAMERA_DISTANCE = 5.0;



////----------------------------------------------------------------------
////! @関数名：FollowCamera
////!
////! @役割：カメラの更新処理
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)							/* デフォルトコンストラクタがないためにコレが必須、らしい */
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

void FollowCamera::Update()
{
	// ゴム紐カメラ設定
	// 視点,参照点
	Vector3 eyepos, refpos;

	// 自機の2m上を参照点に
	refpos = m_targetPos + Vector3(0, 2.0f, 0);
	// 参照点と視点の差分ベクトル
	Vector3 cameraV(0, 0, 5.0f);

	// 自機の後ろに回り込むための回転行列を生成
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);

	// 差分ベクトルを回転させる
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	// 視点座標を計算
	eyepos = refpos + cameraV;

	this->SetEyepos(eyepos);	/* 元はm_cameraの関数を～という意味だが、*/
	this->SetRefpos(refpos);	/* それは今この関数なので、thisで代用可。実はthis->は省略可 */

	// 基底クラスの更新
	Camera::Update();

}



void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(const float & targetAngle)
{
	m_targetAngle = targetAngle;
}


