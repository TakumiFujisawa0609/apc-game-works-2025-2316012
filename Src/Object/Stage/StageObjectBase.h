#pragma once
#include <nlohmann/json.hpp>
#include "../ActorBase.h"


// JSON名前空間
using Json = nlohmann::json;

class StageObjectBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
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
	
	// 初期位置
	const VECTOR INITIAL_POS;

	// 初期角度
	const VECTOR INITIAL_ROT;
	
	// 初期スケール
	const VECTOR INITIAL_SCL;

	// ステージキー
	const std::string STAGE_KEY;

	// 処理の適用処理
	void UpdateApply() override;

	// メインの描画処理
	void DrawMain() override;
};

