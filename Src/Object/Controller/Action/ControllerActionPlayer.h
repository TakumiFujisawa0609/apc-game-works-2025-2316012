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

private:

	// ゲージの最大値
	static constexpr float GAUGE_MAX = 100.0f;

	// レポート入力時間
	const float REPORT_INPUT_TIME;

	// 所有者のインスタンスを参照
	Player& player_;

	// 入力管理クラスの参照
	InputManager& input_;

	// 衝突判定管理クラスの参照
	CollisionManager& collMng_;

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

	// ラインの判定を生成
	void CreateLineCollider();
};