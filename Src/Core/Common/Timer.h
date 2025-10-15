#pragma once

class SceneManager;

class Timer
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="goalTime">初期目標時間</param>
	Timer(const float goalTime = 0.0f);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Timer();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// カウントダウン
	/// </summary>
	/// <returns></returns>
	bool CountDown();

	/// <summary>
	/// 目標時間の設定
	/// </summary>
	/// <param name="goalTime">目標時間</param>
	void SetGoalTime(const float goalTime) { goalTime_ = goalTime; }

private:

	// シーン管理クラスの参照
	SceneManager& scnMng_;

	// 更新ステップ
	float step_;

	// 目標タイム
	float goalTime_;
};