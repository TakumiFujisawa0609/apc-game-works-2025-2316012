#pragma once
#include "../CoreBase.h"

class GameStateManager;

class CoreGameBase : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CoreGameBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CoreGameBase() override;

private:

	// ゲーム状態の管理参照
	GameStateManager& stateMng_;
};