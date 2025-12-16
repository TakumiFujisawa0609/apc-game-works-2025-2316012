#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

class ModelRenderer;
class PixelRenderer;
class BillboardRenderer;

class RendererManager : public Singleton<RendererManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<RendererManager>;

public:

	/// <summary>
	/// テクスチャ種類
	/// </summary>
	enum class TEXTURE_TYPE
	{
		NONE,
		MAIN,			// メインスクリーン
		SHADOW,			// シャドウマップ
		POST_EFFECT,	// ポストエフェクト
	};

	/// <summary>
	/// ポストエフェクトの種類
	/// </summary>
	enum class POST_EFFECT_TYPE
	{
		NONE,
		GRAY_SCALE,		// グレースケール
		INVERSION,		// ネガポジ反転
		BLUR,			// ぼかし
	};

	/// <summary>
	/// シェーダーの種類
	/// </summary>
	enum class SHADER_TYPE
	{
		SHADOW_MESH,
		SHADOW_SKIN_MESH,
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	 
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// シーン遷移時に解放処理
	/// </summary>
	void SceneChangeRelease();

	/// <summary>
	/// ポストエフェクトの種類を変更
	/// </summary>
	/// <param name="postEffectType">ポストエフェクトの種類</param>
	void ChangePostEffect(const POST_EFFECT_TYPE postEffectType);

	/// <summary>
	/// ピクセルレンダラーの追加
	/// </summary>
	/// <param name="pixelRenderer">ピクセルレンダラー</param>
	void AddPixelRenderer(const std::shared_ptr<PixelRenderer> pixelRenderer);

	/// <summary>
	/// モデルレンダラーの追加
	/// </summary>
	/// <param name="pixelRenderer">ピクセルレンダラーの追加</param>
	void AddModelRenderer(const std::shared_ptr<ModelRenderer> pixelRenderer);

	/// <summary>
	/// ビルボードレンダラーの追加
	/// </summary>
	/// <param name="pixelRenderer">ビルボードレンダラー</param>
	void AddBillboardRenderer(const std::shared_ptr<BillboardRenderer> pixelRenderer);

	/// <summary>
	/// ポストエフェクト用のピクセルレンダラーの追加
	/// </summary>
	/// <param name="pixelRenderer">ピクセルレンダラー</param>
	void AddPostEffectRenderer(const POST_EFFECT_TYPE type, const std::shared_ptr<PixelRenderer> pixelRenderer);

	/// <summary>
	/// テクスチャハンドルを返す
	/// </summary>
	/// <param name="textureType">テクスチャ種類</param>
	/// <returns>テクスチャ</returns>
	const int GetTexture(const TEXTURE_TYPE textureType) const;

private:

	// シャドウマップサイズ
	static constexpr int SHADOW_MAP_SIZE = 8224;

	// ポストエフェクト用種類
	POST_EFFECT_TYPE postEffectType_;

	// ピクセルレンダラーの配列
	std::vector<std::shared_ptr<PixelRenderer>> pixelRenderers_;

	// モデルレンダラーの配列
	std::vector<std::shared_ptr<ModelRenderer>> modelRenderers_;

	// ビルボードレンダラーの配列
	std::vector<std::shared_ptr<BillboardRenderer>> billboardRenderers_;

	// ポストエフェクト用ピクセルレンダラー管理マップ
	std::unordered_map<POST_EFFECT_TYPE, std::shared_ptr<PixelRenderer>> postEffectRenderersMap_;

	// テクスチャ管理マップ
	std::unordered_map<TEXTURE_TYPE, int> texturesMap_;

	// コンストラクタ
	RendererManager();

	// デストラクタ
	~RendererManager();
};

