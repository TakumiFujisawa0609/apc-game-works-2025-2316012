#pragma once
#include "StageObjectBase.h"

class ControllerOnHitStageObject;

class StageGimmickObjectBase : public StageObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">ステージハンドル用名前キー</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageGimmickObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageGimmickObjectBase() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider) override;

	/// <summary>
	/// 異変の設定
	/// </summary>
	virtual void SetAnomaly();

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	virtual void Refresh();

	/// <summary>
	/// 色を変更
	/// </summary>
	/// <param name="red">赤値</param>
	/// <param name="green">緑値</param>
	/// <param name="blue">青値</param>
	void SetChangeColor(const int red, const int green, const int blue);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="textureHandle">変更先のテクスチャ</param>
	void ChangeTexture(const int textureHandle, const int textureIndex);

protected:

	// 変更先テクスチャ番号
	int textureIndex_;

	// 変更前のテクスチャ
	int preTexture_;

	// 異変用の判定コライダー
	std::shared_ptr<ColliderBase> anomalyCollider_;

	// 衝突後処理
	std::unique_ptr<ControllerOnHitStageObject> onHit_;
};