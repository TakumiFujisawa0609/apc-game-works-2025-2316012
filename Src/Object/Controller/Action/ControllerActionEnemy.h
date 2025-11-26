 #pragma once
#include "ControllerActionBase.h"

class Enemy;
class ControllerAnimation;
class ControllerPathFinder;
class ControllerCameraJumpScare;
class ColliderSphere;
class ScreenShake;
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
		SEARCH,			// 探索
		IDLE,			// 待機
		CHASE,			// 追跡
		CHASE_NEAR,		// 追跡(至近距離)
		ACTION,			// 攻撃
	};

	// 隣接ノードの距離
	static constexpr float ADJACENT_NODE_DIST = 200.0f;

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
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(const STATE state);

	/// <summary>
	/// 視野範囲内かを設定
	/// </summary>
	/// <param name="isRange">視野範囲内の判定</param>
	void SetIsViewRange(const bool isRange) { isViewRange_ = isRange; }

private:

	// 移動速度
	const float MOVE_SPEED;

	// ダッシュ速度
	const float DASH_SPEED;

	// 視野範囲
	const float VIEW_RANGE;

	// 視野角
	const float VIEW_ANGLE;

	// カメラ位置
	static constexpr VECTOR JUMP_SCARE_CAMERA_POS = { -2.0f, 125.0f, 71.0f };

	// 注視点の位置
	static constexpr VECTOR JUMP_SCARE_CAMERA_TARGET_POS = { 10.0f, 193.0f, -112.0f };

	// カメラトランジションの時間
	static constexpr float TRANSITION_TIME = 0.75f;

	// 目的地の変更時間
	static constexpr float CHANGE_POINT_TIME = 2.0f;

	// 画面シェイク強さ
	static constexpr float SHAKE_POWER = 10.0f;

	// 画面シェイク時間
	static constexpr float SHAKE_TIME = 3.0f;

	// 追跡範囲
	static constexpr float CHASE_RANGE = 1500.0f;

	// 至近距離範囲
	static constexpr float NEAR_RANGE = 300.0f;

	// 目的地到達距離
	static constexpr float GOAL_REACH_DIST = 5.0f;

	// 待機ランダムダイス
	static constexpr int IDLE_RAND = 10;

	// 待機時間
	static constexpr int IDLE_TIME_MIN = 2;	// 最小
	static constexpr int IDLE_TIME_MAX = 5;	// 最大
	static constexpr int IDLE_TIME_RANGE = IDLE_TIME_MAX - IDLE_TIME_MIN; // 範囲

	// 心拍音の最大音量
	static constexpr int HEART_BEAT_MAX_VOLUME = 100;

	// 心拍音の最小音量
	static constexpr int HEART_BEAT_MIN_VOLUME = 50;

	// ガラスが破壊されるまでの時間
	static constexpr float GLASS_BREAK_TIME = 2.2f;

	// 所有者の参照インスタンス
	Enemy& owner_;

	// ターゲット相手のトランスフォーム
	const Transform& targetTransform_;

	// アニメーション
	ControllerAnimation& animation_;

	// 経路探索
	ControllerPathFinder& pathFinder_;

	// 移動位置のリスト
	const std::vector<VECTOR>& movePosList_;

	// ポイント配列
	std::vector<int> points_;

	// 総ポイント数
	int totalPoints_;

	// 最終的な目的地のインデックス
	int goalIndex_;

	// 範囲内か判定
	bool isViewRange_;

	// 次のポイント座標
	VECTOR nextPointPos_;

	// 状態
	STATE state_;

	// タイマー
	std::unique_ptr<Timer> timer_;	
	
	// カメラのトランジション
	std::unique_ptr<ControllerCameraJumpScare> transition_;

	// 画面シェイク
	std::unique_ptr<ScreenShake> shake_;

	// 行動用球体コライダー
	std::shared_ptr<ColliderSphere> colliderSphere_;

	// 状態別更新処理の管理
	std::function<void()> updateFunc_;

	// 状態変更処理を管理するマップ
	std::unordered_map<STATE, std::function<void()>> stateChangeMap_;

	// 状態別更新処理の登録
	void RegisterChangeStateFunction(const STATE state, std::function<void()> func);

	// 状態遷移関数
	void ChangeStateSearch();		// 探索
	void ChangeStateIdle();			// 待機
	void ChangeStateChase();		// 追跡
	void ChangeStateChaseNear();	// 追跡
	void ChangeStateAction();		// 攻撃

	// 状態別更新処理
	void UpdateSearch();			// 探索
	void UpdateIdle();				// 待機
	void UpdateChase();				// 追跡
	void UpdateChaseNear();			// 追跡
	void UpdateAction();			// 攻撃

	// 新しい目的地の設定
	void NewTargetPoint();

	// ターゲットまでの経路を探索
	void FindPathToTarget();

	// 敵の探索
	bool IsSearchTarget();

	// 一定範囲内にターゲットがいるか調べる
	bool CheckRangeToTarget(const float range);

	// 視野判定用のラインコライダーの生成
	void CreateLineCollider(const VECTOR& start, const VECTOR& end);

	// ランダムで目的地のインデックスを返す
	int GetRandGoalIndex();

	// 心拍音の処理
	void HeartBeat();
};