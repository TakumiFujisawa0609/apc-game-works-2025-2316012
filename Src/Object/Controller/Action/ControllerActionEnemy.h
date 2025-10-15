#pragma once
#include "ControllerActionBase.h"

class Enemy;
class ControllerAnimation;
class ControllerPathFinder;
class Timer;

class ControllerActionEnemy : public ControllerActionBase
{
public:
	
	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		MOVE,			// 移動中
		POINT_NEW,		// ポイント更新
		IDLE,			// 待機
		CHASE,			// 追跡
		ACTION,			// 攻撃
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者の参照インスタンス</param>
	ControllerActionEnemy(Enemy& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerActionEnemy() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(const STATE state) { state_ = state; }

private:

	// 移動速度
	const float MOVE_SPEED;

	// ダッシュ速度
	const float DASH_SPEED;

	// 総ポイント数
	//const int TOTAL_POINT_NUM;	
	
	// 隣接ノードの距離
	static constexpr float ADJACENT_NODE_DIST = 150.0f;

	// 待機ランダムダイス
	static constexpr int IDLE_RAND = 10;

	// 待機時間
	static constexpr int IDLE_TIME_MIN = 2;	// 最小
	static constexpr int IDLE_TIME_MAX = 5;	// 最大
	static constexpr int IDLE_TIME_RANGE = IDLE_TIME_MAX - IDLE_TIME_MIN; // 範囲

	// 所有者の参照インスタンス
	Enemy& owner_;

	// アニメーション
	ControllerAnimation& animation_;

	// 経路探索
	ControllerPathFinder& pathFinder_;

	// 移動位置のリスト
	const std::vector<VECTOR>& movePosList_;

	// ポイント配列
	std::vector<int> points_;

	int totalPointList_;

	// 最終的な目的地のインデックス
	int goalIndex_;

	// 現在の目的地の座標
	VECTOR targetPos_;

	// 状態
	STATE state_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 状態別に更新処理を管理するマップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateFuncMap_;

	// 状態別更新処理の登録
	void RegisterUpdateFunction(const STATE state, std::function<void()> func);

	// 状態別更新処理
	void UpdateMove();			// 移動中
	void UpdatePointNew();		// ポイント更新
	void UpdateIdle();			// 待機
	void UpdateChase();			// 追跡
	void UpdateAction();		// 攻撃

	// ランダムで目的地のインデックスを返す
	int GetRandGoalIndex();
};