///-------------------------------------------------------------------
//*
//*	@作成日		2017/05/22
//*
//*	@名前		Obj3d.cpp
//*
//* @役割		3Dオブジェクトのクラス
//*
//*	@著者		加藤 彰馬
//*
///-------------------------------------------------------------------

// ヘッダのインクルード
#include "pch.h"
#include "Obj3d.h"

// 名前空間
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;



// 静的メンバ変数の実体

// デバイス/* cmoの読み込みに必須 */
Microsoft::WRL::ComPtr<ID3D11Device>			Obj3d::m_d3dDevice;
// デバイスコンテキスト/* モデル描画に必須 */
Microsoft::WRL::ComPtr<ID3D11DeviceContext>		Obj3d::m_d3dContext;

// カメラ（ビュー行列と射影行列が入ってる）
Camera*											Obj3d::m_camera;

// 汎用描画ステート
std::unique_ptr<DirectX::CommonStates>			Obj3d::m_states;

// エフェクトファクトリ/* 何かわからんけど必須 */
std::unique_ptr<DirectX::EffectFactory>			Obj3d::m_factory;

/* Obj3d::がないと、ただのグローバル変数状態になるので注意 */



////----------------------------------------------------------------------
////! @関数名：Obj3d
////!
////! @役割：3Dモデルの初期化処理
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
Obj3d::Obj3d()
{
	// スケールは1倍がデフォルト
	m_scale = Vector3(1, 1, 1);

	// 親の3Dオブジェクトをまずは空にしておく
	m_parent = nullptr;

	// 最初はオイラー角で角度を計算
	m_UseQuaternion = false;
}


//
//Obj3d::~Obj3d()
//{
//}


////----------------------------------------------------------------------
////! @関数名：InitializeStatic
////!
////! @役割：静的メンバ変数の初期化処理
////!
////! @引数：なし(void)
////!
////! @戻り値：なし(void)
////----------------------------------------------------------------------
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dContext = d3dContext;
	m_d3dDevice = d3dDevice;
	m_camera = camera;


	// 汎用描画ステートを生成
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());
	// エフェクトファクトリを生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャの読み込みパス指定		/* ここからテクスチャを読み込みますよ、という意味に */
	m_factory->SetDirectory(L"Resources");
}


void Obj3d::LoadModel(const wchar_t * fileName)
{
	// モデルの読み込み
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory);
}

void Obj3d::Update()
{
	/* 行列の計算 */


	// スケーリング行列
	Matrix scalemat = Matrix::CreateScale(m_scale);


	// ↓のif文のrotmatをif文の上で宣言しとけば、さらにその下で使っても怒られない
	Matrix rotmat;
	// 回転行列
	if (m_UseQuaternion)
	{	// クォータニオンで回転を計算
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{	// オイラー角で回転を計算（Z→X→Y）
		/* それぞれの回転分が引数 */
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);

		//* 元々ここで宣言していたがスケール化に伴って上での宣言に
		rotmat = rotmatZ * rotmatX * rotmatY;
	}


	// 平行移動行列
	Matrix transmat = Matrix::CreateTranslation(m_translation);


	// ワールド行列の合成　（親がいるなら、親のワールド行列も忘れずに）
	m_world = scalemat * rotmat * transmat;
	// 親がいるなら、親の行列を掛ける
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}

}

void Obj3d::Draw()
{
	//* 3Dモデルの描画


	// カメラがnullptrだったら強制終了
	assert(m_camera);

	// モデルが存在しているなら、それを描画する
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states,
			m_world,
			m_camera->GetView(),
			m_camera->GetProj());
	}

}
