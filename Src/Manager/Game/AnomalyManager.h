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
		CROWD,			// 群衆
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
		{TYPE::CROWD,"Crowd"},
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

	// 固定イベント種類リスト
	const std::vector<TYPE> TYPE_LIST =
	{ TYPE::PAINTING, TYPE::GRASS_ROOM, TYPE::PAINTING, TYPE::GHOST, TYPE::BLOODY_ROOM, TYPE::CHAIR_MOUNTAIN, TYPE::REVERSE_FALL, TYPE::PAINTING, TYPE::GHOST, TYPE::CROWD, TYPE::PAINTING, TYPE::GHOST, TYPE::BLOODY_ROOM, TYPE::GHOST, TYPE::PAINTING };

	// 固定イベント出現時間リスト
	const std::vector<int> TIME_LIST =
	{ 20, 25, 10, 20, 30, 15, 22, 28, 30, 28, 35, 40, 40, 40, 40 };
	
	// イベントメッセージの遅れ時間
	static constexpr float EVENT_MESSAGE_DELAY_TIME = 5.0f;

	// 異変情報ファイル名
	const std::string ANOMALY_FILE = "Anomalies";		
	
	// 固定イベント用インデックス
	int eventIndex_;	
	
	// 初期異変発生開始時間
	float firstTime_;

	// 異変発生最大時間
	int timeMax_;

	// 異変発生最小時間
	int timeMin_;

	// 発生判定
	bool isOccurrence_;

	// 初期時
	bool isFirst_;

	// 更新種類
	TYPE updateType_;

	// 異変情報
	Json anomalyFile_;

	// 出現処理
	std::function<void()> appearance_;

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

	// 固定で出現
	void FixedAppearance();

	// ランダムで出現
	void RandomApperance();

	// コンストラクタ
	AnomalyManager();

	// デストラクタ
	~AnomalyManager();
};