#pragma once
#include "../ActorBase.h"

class StageObjectBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam);

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

	// 処理の適用処理
	void UpdateApply() override;

	// メインの描画処理
	void DrawMain() override;

	// トランスフォームの初期化
	void InitTransform() override;

	/// <summary>
	/// デバッグ時の描画
	/// </summary>
	void DebugDraw() override;
};

