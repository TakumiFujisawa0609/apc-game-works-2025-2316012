#pragma once
#include "GameStateBase.h"

class Enemy;

class GameStateJumpScare : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStateJumpScare();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStateJumpScare() override;

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

};