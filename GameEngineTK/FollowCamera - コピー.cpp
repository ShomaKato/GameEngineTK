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

void FollowCamera::Update()		/* タンクの移動処理より先に行う */
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
	// カメラ座標を計算
	eyepos = refpos + cameraV;


	//=======================ゴム紐化処理=======================//

	// 視点を現在位置から補間する
	eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;		
	/* 前フレームのカメラ位置がm_eyepos（まだUpdate掛かる前だから）
	　 視点（目的地）と前フレーム視点（現在地）の差分×5％ */
	// 1フレームかけて1進むところを、無理やり0.05だけ進むように矯正してるイメージ？

	/* この状態でも大体できているが、参照点をいじらないと自機が常に画面中央にいるわけではなくなる */



	// 参照点を現在位置から補間する
	refpos = m_refpos + (refpos - m_refpos) * 0.05f;

	/* 上に同じ */

	//==========================================================//


	//=======================fps化処理==========================//

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


