#pragma once
#include <nlohmann/json.hpp>

class CharacterManager;
class StageManager;
class ResourceManager;
class GameSystemManager;
class Player;

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
	/// 読み込み処理
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

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

	// リソース管理クラス
	ResourceManager& resMng_;

	// ゲーム内のシステム管理クラス
	GameSystemManager& systemMng_;
};

