#pragma once
#include <memory>
#include <functional>
#include "CharacterBase.h"

class ControllerOnHitReport;

class Player : public CharacterBase
{
public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		ALIVE,
		DEAD,
	};

	//アニメ種別キー
	static const std::string ANIM_JUMP;		//ジャンプ
	static const std::string ANIM_DIE;		//死ぬ
	static const std::string ANIM_SLEEP;	//眠る

	// 最大狂気値
	static constexpr int MADNESS_MAX = 100;	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター</param>
	Player(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 状態変更処理
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(const STATE state) { state_ = state; }

	/// <summary>
	/// ジャンプ量を返す
	/// </summary>
	/// <returns>ジャンプ量</returns>
	const float GetJumpAmount() const { return JUMP_AMOUNT; }

	/// <summary>
	/// ジャンプ受付時間を返す
	/// </summary>
	/// <returns>ジャンプ受付時間</returns>
	const float GetJumpAcceptTime() const { return JUMP_ACCEPT_TIME; }

	/// <summary>
	/// レポートの入力時間を返す
	/// </summary>
	/// <returns>レポート入力時間</returns>
	const float GetReportTime() const { return REPORT_INPUT_TIME; }

	/// <summary>
	/// 狂気値の追加値を返す
	/// </summary>
	/// <returns>狂気値の追加値</returns>
	const float GetMadnessUpdateStep() const { return madnessUpdateStep_; }

	/// <summary>
	/// ジャンプ用ステップを返す
	/// </summary>
	/// <returns>ジャンプ用ステップ</returns>
	const float GetStepJump() const { return stepJump_; }

	/// <summary>
	/// レポートの報告率
	/// </summary>
	/// <returns>報告率</returns>
	const float GetReportPercent() const { return reportPer_; }

	/// <summary>
	/// 狂気値を返す
	/// </summary>
	/// <returns>狂気値</returns>
	const int GetMadnessValue() const { return madnessValue_; }

	/// <summary>
	/// ジャンプ判定を返す
	/// </summary>
	/// <returns>ジャンプ判定</returns>
	const bool IsJump() const { return isJump_; }

	/// <summary>
	/// ジャンプ判定を設定
	/// </summary>
	/// <param name="isJump">ジャンプ判定</param>
	void SetIsJump(const bool isJump) { isJump_ = isJump; }

	/// <summary>
	/// ジャンプ用ステップを設定
	/// </summary>
	/// <param name="step">ジャンプ用ステップ</param>
	void SetStepJump(const float step) { stepJump_ = step; }

	/// <summary>
	/// レポートの進捗率設定
	/// </summary>
	/// <param name="percent">進捗率</param>
	void SetReportPercent(const float percent) { reportPer_ = percent; }

	/// <summary>
	/// 狂気値を設定
	/// </summary>
	/// <param name="madnessValue">狂気値</param>
	void SetMadnessValue(const int madnessValue);

	/// <summary>
	/// 狂気値を追加
	/// </summary>
	/// <param name="madnessValue">狂気値</param>
	void AddMadnessValue(const int madnessValue);

	/// <summary>
	/// 狂気更新用ステップの追加
	/// </summary>
	void AddMadnessUpdateStep() { madnessUpdateStep_ += MADNESS_UPDATE_STEP_ADD; }

	/// <summary>
	/// 狂気更新用ステップの減少
	/// </summary>
	void SubMadnessUpdateStep() { madnessUpdateStep_ -= MADNESS_UPDATE_STEP_ADD; }

private:

	// ジャンプ量
	const float JUMP_AMOUNT;

	// ジャンプ受付時間
	const float JUMP_ACCEPT_TIME;

	// ジャンプアニメーション
	const float ANIM_JUMP_SPEED;

	// レポート進捗率
	const float REPORT_INPUT_TIME;	
	
	// 狂気更新用ステップ値の追加値
	const float MADNESS_UPDATE_STEP_ADD;

	// 狂気値
	int madnessValue_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプ用ステップ
	float stepJump_;

	// レポート報告率
	float reportPer_;

	// 狂気更新用ステップ値
	float madnessUpdateStep_;

	// 状態
	STATE state_;

	// 状態別の更新関数マップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateFuncMap_;

	// メインの更新処理
	void UpdateBody() override;	

	//　メインの描画処理
	void DrawMain() override;

	// アニメーションの初期化
	void InitAnimation() override;

	// トランスフォームの初期化
	void InitTransform() override;
	
	// 状態別更新処理の登録
	void RegisterStateUpdateFunc(const STATE state, std::function<void()> update);

	// 状態別更新処理	
	void UpdateNone() {};	// 何もしない
	void UpdateAlive();		// 生存状態の更新
	void UpdateDead();		// 死亡状態の更新

	// デバッグ描画
	void DebugDraw() override;
};