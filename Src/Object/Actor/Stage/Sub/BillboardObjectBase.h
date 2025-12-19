#pragma once
#include <memory>
#include <DxLib.h>
#include "../../../../Common/Vector2.h"

class SceneManager;
class ResourceManager;
class ModelMaterial;
class BillboardRenderer;

class BillboardObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	BillboardObjectBase(const VECTOR& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BillboardObjectBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <returns>座標</returns>
	const VECTOR& GetPos() const { return pos_; }

protected:

	// シーン管理クラス
	SceneManager& scnMng_;

	// リソース管理クラス
	ResourceManager& resMng_;

	// ステップ
	float step_;

	// 活動状態
	bool isActive_;

	// 座標
	VECTOR pos_;

	// サイズ
	Vector2 size_;

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<BillboardRenderer> renderer_;

	// バッファーの更新
	virtual void UpdateBuffer();
};