#pragma once
#include <memory>
#include "SceneBase.h"

class ScenePause;
class GameStateBase;
class TestModel;
class CreatePositionList;

class SceneGame : public SceneBase
{

public:
	
	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		PLAY,		// プレイ
		REPORTING,	// 報告
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneGame();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="state">状態</param>
	void ChangeState(const STATE state) { state_ = state; }

private:

	// 状態
	STATE state_;

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// 状態別処理の管理マップ
	std::unordered_map<STATE, std::unique_ptr<GameStateBase>> stateMap_;

	// テスト
	std::unique_ptr<TestModel> test_;

	// ポジションリスト作成
	std::unique_ptr<CreatePositionList> createPositionList_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};