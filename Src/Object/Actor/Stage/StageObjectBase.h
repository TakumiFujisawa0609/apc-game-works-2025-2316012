#pragma once
#include "../ActorBase.h"

class StageObjectBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">ステージハンドル用名前キー</param>
	/// <param name="param">パラメーター情報</param>
	StageObjectBase(const std::string& key, const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageObjectBase();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() override;

protected:

	// ステージキー
	const std::string STAGE_KEY;

	// 1メートルをゲーム内の単位に変換するスケール
	static constexpr float METER_TO_UNIT_SCALE = 100.0f;	
	
	// コライダー半径
	float colliderRadius_;

	// コライダー頭部座標
	VECTOR colliderHeadPos_;

	// コライダー末尾座標
	VECTOR colliderEndPos_;

	// 処理の適用処理
	void UpdateApply() override;

	// メインの描画処理
	void DrawMain() override;

	// トランスフォームの初期化
	void InitTransform() override;

	// コライダーの生成
	virtual void MakeCollider() override;

	/// <summary>
	/// デバッグ時の描画
	/// </summary>
	void DebugDraw() override;
};

