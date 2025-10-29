#pragma once
#include "ControllerActionBase.h"

class Player;
class InputManager;
class CollisionManager;

class ControllerActionPlayer : public ControllerActionBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">所有者(プレイヤー)</param>
	ControllerActionPlayer(Player& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerActionPlayer() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグの描画処理
	/// </summary>
	void DebugDraw() override;

private:

	// ゲージの最大値
	static constexpr float GAUGE_MAX = 100.0f;

	// レポートの範囲
	static constexpr float REPORT_RANGE = 1000.0f;

	// 始点調整用
	static constexpr float OFFSET_Y = 120.0f;

	// 狂気値の追加量
	static constexpr int MADNESS_ADD_VALUE = 1;

	// レポート入力時間
	const float REPORT_INPUT_TIME;

	// 回転時間
	const float TIME_ROT;

	// 所有者のインスタンスを参照
	Player& player_;

	// 入力管理クラスの参照
	InputManager& input_;

	// 回転用ステップ
	float stepRotTime_;

	// 狂気値用ステップ
	float madnessStep_;

	// 着地判定
	bool isEndLanding_;

	// 移動操作
	void ProcessMove();

	// ジャンプ操作
	void ProcessJump();

	// レポート処理
	void ProcessReport();

	// 着地判定
	bool IsEndLanding() const;

	// 目的回転角度の設定
	void SetGoalRotate(const double rotRad);

	// ラインの判定を生成
	void CreateLineCollider();

	// 狂気値の影響から速度を取得
	const float GetApplyMadnessToSpeed(float speed);
};