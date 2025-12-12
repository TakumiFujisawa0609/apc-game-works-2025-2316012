#pragma once
#include <nlohmann/json.hpp>
#include "../CoreBase.h"

class SoundManager;
class GameStateManager;
class GameSystemManager;
class InputManager;

// JSON名前空間
using Json = nlohmann::json;

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

	/// <summary>
	/// 活動状態の判定
	/// </summary>
	/// <returns></returns>
	const bool IsActive() const { return isActive_; }

	/// <summary>
	/// 活動状態の設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

protected:

	// サウンド管理クラスの参照
	SoundManager& sndMng_;

	// ゲーム状態の管理参照
	GameStateManager& stateMng_;

	// システム管理クラスの参照
	GameSystemManager& systemMng_;

	// 入力管理クラスの参照
	InputManager& inputMng_;

	// 活動状態
	bool isActive_;
};