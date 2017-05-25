///-------------------------------------------------------------------
//*
//*	@作成日		2017/05/22
//*
//*	@名前		Obj3d.h
//*
//* @役割		3Dオブジェクトのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------
// 多重インクルードの防止
#pragma once

// ヘッダファイルのインクルード
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	// 静的メンバ変数の初期化を行う変数（外部から渡すものは引数に）
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>	d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
		Camera* camera);
	/* 渡すのはデバイス、デバイスコンテキスト、カメラ */

private:
	//==========静的メンバ変数（インスタンスがたくさんあってもこれらは一つ）==========//

	// デバイス		/* cmoの読み込みに必須 */
	static Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	// デバイスコンテキスト		/* モデル描画に必須 */
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

	// カメラ（ビュー行列と射影行列が入ってる）
	static Camera* m_camera;
																
	// 汎用描画ステート
	static std::unique_ptr<DirectX::CommonStates> m_states;

	// エフェクトファクトリ/* 何かわからんけど必須 */
	static std::unique_ptr<DirectX::EffectFactory> m_factory;




public:
	Obj3d();	/* 全部セッターゲッターで受け取るので、デフォルトコンストラクタでおｋ */
	~Obj3d();

	// モデルをロード
	void LoadModel(const wchar_t* fileName);	/* ファイル名はCreateFromCMOの引数をパクる */

	// 更新
	void Update();

	// 描画
	void Draw();


	//==========setter==========//
	// スケーリング(XYZ)	/* 初期値０だと０倍表示しようとするので、1,1,1にしとくとよし */
	void SetScale(DirectX::SimpleMath::Vector3&	scale) { m_scale = scale; }

	// 回転角(Z,Y,X)
	void SetRotation(DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }

	// 平行移動(X,Y,Z)
	void SetTranslation(DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }

	// 親の3Dオブジェクトのポインタ
	void SetParent(Obj3d* parent) { m_parent = parent; }

	//==========getter==========//
	// スケーリング(XYZ)
	const DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	// 回転角(Z,Y,X)
	const DirectX::SimpleMath::Vector3 GetRotation() { return m_rotation; }

	// 平行移動(X,Y,Z)
	const DirectX::SimpleMath::Vector3 GetTranslation() { return m_translation; }

	// ワールド行列
	const DirectX::SimpleMath::Matrix GetWorld() { return m_world; }

	// 親の3Dオブジェクトのポインタ
	const Obj3d* GetParent() { return m_parent; }


private:
	//==========メンバ変数==========//
	// 3Dモデル
	std::unique_ptr<DirectX::Model> m_model;

	// スケーリング(XYZ)
	DirectX::SimpleMath::Vector3	m_scale;	/* Vector3は最初から(0,0,0)が入ってるので、 */
												/* scaleだけは初期化で変えないとまずい。 */
	// 回転角(Z,Y,X)
	DirectX::SimpleMath::Vector3	m_rotation;

	// 平行移動(X,Y,Z)
	DirectX::SimpleMath::Vector3	m_translation;

	// ワールド行列
	DirectX::SimpleMath::Matrix		m_world;

	// （親が存在するならば）親パーツの3Dオブジェクト、のポインタ
	Obj3d*							m_parent;
};