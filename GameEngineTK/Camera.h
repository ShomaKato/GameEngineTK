///-------------------------------------------------------------------
//*
//*	@作成日		2017/05/11
//*
//*	@名前		Camera.h
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// 多重インクルードの防止
#pragma once

// ヘッダファイルのインクルード
#include <d3d11.h>
#include <SimpleMath.h>



class Camera
{
public:
	// メンバ関数はここに //
	Camera(int width,int height);
	virtual ~Camera();

	// カメラの更新関数
	virtual void Update();


	// 視点をセットするための関数
	void SetEyepos(DirectX::SimpleMath::Vector3 eyepos);

	// 注視点をセットするための関数
	void SetRefpos(DirectX::SimpleMath::Vector3 refpos);

	// 上方向ベクトルをセットするための関数
	void SetUpvec(DirectX::SimpleMath::Vector3 Upvec);

	//// ビュー行列を生成するための関数
	//void SetViewMatrix();					/* 上三つを変更すれば、Updateで変わるので不要 */

	// ビュー行列を取得する関数
	const DirectX::SimpleMath::Matrix& GetView();
	/* const参照渡しにしたほうが軽くなるらしい */



	// 垂直方向視野角をセットするための関数
	void SetFovY(float fovY);

	// 注視点をセットするための関数
	void SetAspect(float aspect);

	// 手前の表示限界（ニアクリップ）をセットするための関数
	void SetNearclip(float nearclip);

	// 奥の表示限界（ファークリップ）をセットするための関数
	void SetFarclip(float farclip);

	// プロジェクション行列を取得する関数
	const DirectX::SimpleMath::Matrix& GetProj();
	/* 上に同じ */




protected:
	// 材料はここにメンバ変数として書く //


	//*==============ビュー行列（カメラの設定用行列）

	// ビュー行列を変換
	/* 視点（どこから見るか） */
	DirectX::SimpleMath::Vector3 m_eyepos;
	/* 注視点（どこを見るか。0,0,0だと原点に） */
	DirectX::SimpleMath::Vector3 m_refpos;
	/* 上方向ベクトル（どちらが画面上か） */
	DirectX::SimpleMath::Vector3 m_Upvec;

	// ビュー行列の生成
	DirectX::SimpleMath::Matrix m_view;





	//*==============プロジェクション行列（カメラの調整用行列）

	/* 垂直方向視野角（60度まで画面に収める、という意味に） */
	float m_fovY;
	/* 縦横の比率。float変換した横幅割る縦幅でおｋ？ */
	float m_aspect;	
	/* 手前の表示限界距離 */
	float m_nearclip;	
	/* 奥の表示限界距離 */
	float m_farclip;	
	// 射影行列の透視投影（生成）
	DirectX::SimpleMath::Matrix m_proj;


	//* MEMO
	//  視点 == 参照点だと落ちる。
	//  この二つの差でカメラの角度が決まるため。
};
