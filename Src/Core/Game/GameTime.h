#pragma once
#include <memory>
#include "CoreGameBase.h"

class Timer;
class GameStateManager;

class GameTime : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameTime(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameTime() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// フォント名
	const std::string FONT_NAME;

	// フォントサイズ
	const int FONT_SIZE;

	// フォント太さ
	const int FONT_THICK;

	// ゲーム時間
	const float GAME_TIME;

	// 日付の表示位置
	const Vector2 DATE_POS;

	// 時間の表示位置
	const Vector2 TIME_POS;

	// イベント時間リスト
	const std::vector<int> EVENT_TIME_LIST;

	// 一分の長さ
	static constexpr int ONE_MINUTES = 60;

	// タスクイベントの発生遅れ時間
	static constexpr float TASK_EVENT_DELAY_TIME = 5.0f;

	// イベントフラグ
	bool isEvent_;

	// 敵の出現フラグ
	bool isEnemyAppear_;

	// イベントステップ
	int eventStep_;

	// イベント処理
	std::function<void()> eventFunc_;

	// 状態管理クラスの参照
	GameStateManager& stateMng_;

	// 日付情報
	CharacterString todayText_;

	// 時間情報
	CharacterString timeText_;

	// 時間
	std::unique_ptr<Timer> timer_;

	// 年月日の文字列を返す
	std::wstring GetYmdWstring();

	// 各種イベント処理
	void EventNone() {};
	void EventStartExplan();
	void EventNightViewExplan();	
	void EventSanityExplan();
	void EventOneMinutesLater();
	void EventEnemyAppear();
	void EventFinalWarning();
};