#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../../Template/Singleton.h"

class Timer;
class AnomalyBase;

// JSON名前空間
using Json = nlohmann::json;

class AnomalyManager : public Singleton<AnomalyManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<AnomalyManager>;

public:

	enum class TYPE
	{
		GHOST,			// ゴーストの出現
		PAINTING,		// 絵の切り替え
		REPLACE,		// 置き換え
		OCCUR,			// 何かが起こる
		COLOR_CHANGE,	// 色変え
		APPEARANCE,		// 出現
		MAX
	};

	// キャラクターの名前リスト
	const std::vector<std::string> ANOMALY_LIST =
	{
		"Ghost",
		"Painting",
		"Replace",
		"Occur",
		"ColorChange",
		"Appearance"
	};
	
	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void DebugDraw();

private:

	static constexpr float FIRST_TIME = 50.0f;
	static constexpr int ANOMALY_TIME_MAX = 40;
	static constexpr int ANOMALY_TIME_MIN = 20;

	// 異変情報ファイル名
	const std::string ANOMALY_FILE = "GhostRespown";

	// 異変情報
	Json anomalyFile_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 異変の管理マップ
	std::unordered_map<TYPE, std::unique_ptr<AnomalyBase>> anomalyMap_;

	// 異変の発生
	void OccurAnomaly(const TYPE type);

	// コンストラクタ
	AnomalyManager();

	// デストラクタ
	~AnomalyManager();
};

