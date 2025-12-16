#pragma once
#include "../Template/Singleton.h"

// 以下のクラスが生成されてからShadowManagerを生成する
class SceneManager;
class CharacterManager;
class StageManager;

class ShadowManager : public Singleton<ShadowManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<ShadowManager>;

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// シャドウマップテクスチャを返す
	/// </summary>
	/// <returns>シャドウマップテクスチャ</returns>
	const int GetShadowMapTexture() const { return shadowMapTexture_; }

	/// <summary>
	/// ライトのビュー行列を返す
	/// </summary>
	/// <returns>ライトのビュー行列</returns>
	const MATRIX& GetLightViewMatrix() const { return lightViewMatrix_; }
	
	/// <summary>
	/// ライトのプロジェクション行列を返す
	/// </summary>
	/// <returns>ライトのプロジェクション行列</returns>
	const MATRIX& GetLightProjectionMatrix() const { return lightProjectionMatrix_; }

private:

	// 色深度
	static constexpr int COLOR_BIT_DEPTH = 24;

	// シャドウマップサイズ
	static constexpr int SHADOW_MAP_SIZE = 8192;

	// 通常のチャンネル数
	static constexpr int DEFAULT_CHANNEL_NUM = 4;

	// シーン管理クラスの参照
	SceneManager& scnMng_;

	// キャラクター管理クラスの参照
	CharacterManager& charaMng_;

	// ステージ管理クラスの参照
	StageManager& stageMng_;

	// シャドウマップ用テクスチャ
	int shadowMapTexture_;

	// シャドウマップ生成用シェーダーハンドル
	int shadowPs_;					// ピクセルシェーダー
	int shadowMeshVs_;				// 頂点シェーダー
	int shadowSkinnedMeshVs_;		// 頂点シェーダー（スキニングメッシュ用）

	// ライトのビュー行列
	MATRIX lightViewMatrix_;

	// プロジェクション行列
	MATRIX lightProjectionMatrix_;

	// シェーダーリセット
	void ResetShader();

	// コンストラクタ
	ShadowManager();

	// デストラクタ
	~ShadowManager();
};