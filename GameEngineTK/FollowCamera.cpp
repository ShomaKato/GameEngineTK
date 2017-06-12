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
using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的な云々かんぬん
//const float CAMERA_DISTANCE = 5.0;



////----------------------------------------------------------------------
////! @関数名：FollowCamera
////!
////! @役割：カメラの初期化処理
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

	m_keyboard = nullptr;

	m_isFPS = false;

	m_player = nullptr;
}

void FollowCamera::Update()		/* 自機の移動処理より先に行う */
{

	//=======================キーボード処理==========================//

	// キーボードの状態取得
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardstate);

	// Cキーを押すごとにFPS/TPSを切り替え
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		// フラグを切り替え
		m_isFPS = !m_isFPS;
	}

	//=======================プレイヤ処理==========================//
	if (m_player)
	{
		// 目標座標の設定
		SetTargetPos(m_player->GetPlayerTranslation());
		SetTargetAngle(m_player->GetPlayerRotationY());
	}


	//=======================カメラ処理==========================//
	if (!m_isFPS)
	{
		// トリガーがfalseならTPSカメラ設定
		// 視点,参照点			/* 視点は「カメラ位置」　参照点は「カメラが中央に捉える位置」 */
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
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.15f;
		/* 前フレームのカメラ位置がm_eyepos（まだUpdate掛かる前だから）
		　 視点（目的地）と前フレーム視点（現在地）の差分×5％ */
		  // 1フレームかけて1進むところを、無理やり0.05だけ進むように矯正してるイメージ？

		  /* この状態でも大体できているが、参照点をいじらないと自機が常に画面中央にいるわけではなくなる */


		  // 参照点を現在位置から補間する
		refpos = m_refpos + (refpos - m_refpos) * 0.05f;

		/* 上に同じ */		// だが、なーんか動きが変。要検証

		//==========================================================//

		this->SetEyepos(eyepos);	/* 元はm_cameraの関数を～という意味だが、*/
		this->SetRefpos(refpos);	/* それは今この関数なので、thisで代用可。実はthis->は省略可 */


	}


	//=======================FPS化処理==========================//

	if (m_isFPS)
	{
		// トリガーがtrueならFPSカメラ設定
		// 視点,参照点
		Vector3 eyepos, refpos;

		// 参照点にしてた場所（自機場所）が視点になればいいので、refposをeyeposに変え、高さを調節
		eyepos = m_targetPos + Vector3(0, 0.5f, 0);
		// 視点と参照点の差分ベクトル。TPSと逆
		Vector3 cameraV(0, 0, -5);

		// 自機の後ろに回り込むための回転行列を生成
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);

		// 差分ベクトルを回転させる
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// カメラ座標を計算。計算するのは参照点座標
		refpos = eyepos + cameraV;

		/* Game.cppの、自機描画部分を消せばよりそれっぽく */

		this->SetEyepos(eyepos);	/* 元はm_cameraの関数を～という意味だが、*/
		this->SetRefpos(refpos);	/* それは今この関数なので、thisで代用可。実はthis->は省略可 */

	}

	//============================================================//

	// 基底クラスの更新
	Camera::Update();

}


////----------------------------------------------------------------------
////! @関数名：SetTargetPos
////!
////! @役割：自機の座標を確認する
////!
////! @引数：自機の座標(&Vector3)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetPos)
{
	m_targetPos = targetPos;
}

////----------------------------------------------------------------------
////! @関数名：SetTargetAngle
////!
////! @役割：自機の角度を確認する
////!
////! @引数：自機の角度(&float)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void FollowCamera::SetTargetAngle(const float & targetAngle)
{
	m_targetAngle = targetAngle;
}



////----------------------------------------------------------------------
////! @関数名：SetKeyboard
////!
////! @役割：キーボードの値を外部から受け取る
////!
////! @引数：キーボードの状態(Keyboard)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}


