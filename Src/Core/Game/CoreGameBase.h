#pragma once
#include "../CoreBase.h"

class SoundManager;
class GameStateManager;
class GameSystemManager;
class InputManager;

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

protected:

	// サウンド管理クラスの参照
	SoundManager& sndMng_;

	// ゲーム状態の管理参照
	GameStateManager& stateMng_;

	// システム管理クラスの参照
	GameSystemManager& systemMng_;

	// 入力管理クラスの参照
	InputManager& inputMng_;
};