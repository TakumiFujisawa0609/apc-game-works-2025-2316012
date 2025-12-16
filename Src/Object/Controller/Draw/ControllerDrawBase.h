#pragma once
#include <memory>
#include <DxLib.h>
#include "../ControllerBase.h"

class ResourceManager;
class CharacterManager;
class ShadowManager;
class ModelMaterial;
class ModelRenderer;

class ControllerDrawBase : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerDrawBase(const int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerDrawBase() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="texture"></param>
	virtual void SetTexture(const int texture);

protected:

	// インスタンスの参照
	ResourceManager& resMng_;
	CharacterManager& charaMng_;
	ShadowManager& shadowMng_;

	// 環境光
	static constexpr VECTOR AMBIENT = { 0.5, 0.5, 0.5 };

	// テクスチャアニメインデックス
	static constexpr int TEX_ANIM_INDEX = 7;

	// テクスチャシャドウインデックス
	static constexpr int TEX_SHADOW_INDEX = 8;

	// 影用のマトリックスバッファーサイズ
	static constexpr int BUFFER_MATRIX_SIZE = 2;

	// モデル
	const int model_;

	// バッファーの更新
	virtual void UpdateBuffer();

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<ModelRenderer> renderer_;
};

