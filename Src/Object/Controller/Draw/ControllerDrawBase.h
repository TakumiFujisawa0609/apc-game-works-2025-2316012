#pragma once
#include <memory>
#include <DxLib.h>
#include "../ControllerBase.h"

class ResourceManager;
class CharacterManager;
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

	// 環境光
	//const VECTOR AMBIENT = { 0.02, 0.02, 0.05 };
	const VECTOR AMBIENT = { 0.5, 0.5, 0.5 };

	// モデル
	const int model_;

	// バッファーの更新
	virtual void UpdateBuffer();

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<ModelRenderer> renderer_;
};

