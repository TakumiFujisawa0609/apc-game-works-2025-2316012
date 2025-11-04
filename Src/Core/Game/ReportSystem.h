#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include "CoreGameBase.h"
#include "../../Common/CharacterString.h"
#include "../Common/CircleGauge.h"

class GameStateManager;
class Timer;
class ControllerTextAnimation;
class Player;

class ReportSystem : public CoreGameBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		WAIT,		// 待機
		REPORTING,	// 報告中
	};

	// ゲージの最大値
	static constexpr float GAUGE_MAX = 100.0f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ReportSystem(Player& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReportSystem() override;

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
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 報告処理へ変更
	/// </summary>
	void ChangeReporting();

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns>状態</returns>
	const STATE GetState() const { return state_; }

private:

	// 報告中の時間
	static constexpr float REPORTING_TIME = 3.0f;

	// 完了時のテキスト表示時間
	static constexpr float TEXT_DISPLAY_TIME = 2.0f;

	// 報告中のテキスト
	const std::wstring REPORTING_TEXT = L"報告中";

	// プレイヤークラスの参照
	Player& player_;

	// 状態
	STATE state_;

	// レポートの進捗率
	float reportPercent_;

	// コンマ用ステップ
	float commaStep_;

	// サークルゲージ
	CircleGauge gauge_;

	// 報告中時のテキスト
	CharacterString reportingText_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 状態別更新処理管理マップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateMap_;	

	// 状態別描画処理管理マップ
	std::unordered_map<STATE, std::function<void()>> stateDrawMap_;	

	// 処理の登録
	void RegisterStateFunction(const STATE state, std::function<void()> update, std::function<void()> draw);

	// 状態別更新処理
	void UpdateWait();
	void UpdateReporting();

	// 状態別描画処理
	void DrawWait();
	void DrawReporting();
};