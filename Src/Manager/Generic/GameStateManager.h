#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

class GameStateBase;

class GameStateManager : public Singleton<GameStateManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<GameStateManager>;

public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		PLAY,
		REPORTING,
		JUMP_SCARE,
		MADNESS_END,
	};

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 状態遷移
	/// </summary>
	void ChangeState(const STATE state);

	/// <summary>
	/// ゲームクリアの設定
	/// </summary>
	void SetGameClear();

	/// <summary>
	/// ゲームオーバーの設定
	/// </summary>
	void SetGameOver();

private:

	// 状態
	STATE state_;

	// 状態別処理の管理マップ
	std::unordered_map<STATE, std::unique_ptr<GameStateBase>> stateMap_;

	// コンストラクタ
	GameStateManager();

	// デストラクタ
	~GameStateManager();
};