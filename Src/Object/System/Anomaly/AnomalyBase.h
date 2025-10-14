#pragma once
#include <nlohmann/json.hpp>

class CharacterManager;
class StageManager;

// JSON名前空間
using Json = nlohmann::json;

class AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AnomalyBase();

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
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	virtual void Occurrence(Json& param);

protected:

	// キャラクター管理クラス
	CharacterManager& charaMng_;

	// ステージ管理クラス
	StageManager& stageMng_;
};

