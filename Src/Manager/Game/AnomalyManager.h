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

	/// <summary>
	/// 種類
	/// </summary>
	enum class TYPE
	{
		GHOST,			// ゴーストの出現
		PAINTING,		// 絵の切り替え
		CHAIR_MOUNTAIN,	// 椅子の山を置く
		BLOODY_ROOM,	// 血の部屋
		REPLACE,		// 置き換え
		OCCUR,			// 何かが起こる
		COLOR_CHANGE,	// 色変え
		APPEARANCE,		// 出現
		GRASS_ROOM,		// 草の部屋
		REVERSE_FALL,	// 反転落下
		MAX
	};

	// キャラクターの名前リスト
	const std::unordered_map<TYPE ,std::string> ANOMALY_LIST =
	{
		{TYPE::GHOST,"Ghost"},
		{TYPE::PAINTING,"Painting"},
		{TYPE::CHAIR_MOUNTAIN,"ChairMountain"},
		{TYPE::BLOODY_ROOM,"BloodyRoom"},
		{TYPE::REPLACE,"Replace"},
		{TYPE::OCCUR,"Occur"},
		{TYPE::COLOR_CHANGE,"ColorChange"},
		{TYPE::APPEARANCE,"Appearance"},
		{TYPE::GRASS_ROOM,"GrassRoom"},
		{TYPE::REVERSE_FALL,"ReverseFall"},
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
	/// 発生判定の設定
	/// </summary>
	/// <param name="isOccurrence">発生判定</param>
	void SetIsOccurrence(const bool isOccurrence) { isOccurrence_ = isOccurrence; }

	/// <summary>
	/// 描画処理
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// 更新処理の種類を設定
	/// </summary>
	/// <param name="type">種類</param>
	void SetUpdateType(const TYPE type) { updateType_ = type; }

private:

	// 異変情報ファイル名
	const std::string ANOMALY_FILE = "Anomalies";	
	
	// 初期異変発生開始時間
	float firstTime_;

	// 異変発生最大時間
	int timeMax_;

	// 異変発生最小時間
	int timeMin_;

	// 発生判定
	bool isOccurrence_;

	TYPE updateType_;

	// 異変情報
	Json anomalyFile_;

	// 各異変ごとの確率重みマップ
	std::unordered_map<TYPE, int> anomalyWeightMap_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 異変の管理マップ
	std::unordered_map<TYPE, std::unique_ptr<AnomalyBase>> anomalyMap_;

	// 異変の発生
	void OccurAnomaly(const TYPE type);

	// ランダムで異変の種類を返す
	const TYPE GetRandType();

	// コンストラクタ
	AnomalyManager();

	// デストラクタ
	~AnomalyManager();
};