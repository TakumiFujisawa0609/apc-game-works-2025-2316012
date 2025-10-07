#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include "../../../Common/CharacterString.h"
#include "../../CoreBase.h"
#include "../../Common/CircleGauge.h"

class InputManager;
class Timer;
class ControllerTextAnimation;

class ReportSystem : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ReportSystem();

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

private:

	// 報告中の時間
	static constexpr float REPORTING_TIME = 3.0f;

	// 完了時のテキスト表示時間
	static constexpr float COMPLITE_TEXT_DISPLAY_TIME = 2.0f;

	// 報告中のテキスト
	const std::wstring REPORTING_TEXT = L"報告中";

	// 完了時のテキスト
	const std::wstring COMPLITE_TEXT = L"異変が報告されました";

	// 失敗時のテキスト
	const std::wstring MISS_TEXT = L"誤った報告がさえれました";

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		WAIT,		// 待機
		REPORT,		// 報告
		REPORTING,	// 報告中
		MISS,		// 失敗
		COMPLETE,	// 完了
	};

	// 入力管理クラスの参照
	InputManager& input_;

	// 状態
	STATE state_;

	// コンマ用ステップ
	float commaStep_;

	// サークルゲージ
	CircleGauge gauge_;

	// 報告中時のテキスト
	CharacterString reportingText_;

	// 完了時のテキスト
	CharacterString compliteText_;

	// 失敗時のテキスト
	CharacterString missText_;

	// テキストアニメーションコントローラー
	std::unique_ptr<ControllerTextAnimation> textAnimation_;

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
	void UpdateReport();
	void UpdateReporting();
	void UpdateMiss();
	void UpdtateComplete();

	// 状態別描画処理
	void DrawWait();
	void DrawReport();
	void DrawReporting();
	void DrawMiss();
	void DrawComplite();

	// 判定の生成
	void CreateLineCollider();

};