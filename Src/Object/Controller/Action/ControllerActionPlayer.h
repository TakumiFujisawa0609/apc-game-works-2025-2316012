#pragma once
#include "ControllerActionBase.h"

class Player;
class InputPlayer;

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

	// 所有者のインスタンスを参照
	Player& player_;

	// 着地判定
	bool isEndLanding_;

	// 入力関連の処理
	std::unique_ptr<InputPlayer> input_;

	// 移動操作
	void ProcessMove();

	// ジャンプ操作
	void ProcessJump();

	// 着地判定
	bool IsEndLanding() const;
};