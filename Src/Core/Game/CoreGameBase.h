#pragma once
#include "../CoreBase.h"

class SoundManager;
class GameStateManager;
class GameSystemManager;

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
};