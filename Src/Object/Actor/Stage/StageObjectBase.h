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

	// 処理の適用処理
	void UpdateApply() override;

	// メインの描画処理
	void DrawMain() override;
};

