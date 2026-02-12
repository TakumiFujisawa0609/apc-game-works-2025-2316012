#pragma once
#include <functional>
#include <unordered_map>
#include "SceneBase.h"

class SceneExplanation : public SceneBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneExplanation();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneExplanation() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init()override {};

	/// <summary>
	/// 更新処理
	/// </summary>
	void NormalUpdate()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void NormalDraw()override;

private:

	// 状態
	enum class STATE
	{
		SELECT,					// 操作説明か遊び方説明か選ぶ画面
		OPERATION_EXPLANATION,	// 操作説明画面
		PLAY_EXPLANATION,		// 遊び方説明画面
	};

	// 現在の状態
	STATE state_;	
	
	// 更新処理
	std::function<void()> update_;

	// 描画処理
	std::function<void()> draw_;

	// 状態遷移処理の管理
	std::unordered_map<STATE, std::function<void()>> stateChangeMap_;

	// 各状態の更新処理
	void UpdateSelect();
	void UpdateOperationExplanation();
	void UpdatePlayExplanation();

	// 各状態の描画処理
	void DrawSelect();
	void DrawOperationExplanation();
	void DrawPlayExplanation();

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateSelect();
	void ChangeStateOperationExplanation();
	void ChangeStatePlayExplanation();
};