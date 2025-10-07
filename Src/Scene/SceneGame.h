#pragma once
#include <memory>
#include "SceneBase.h"

class ScenePause;
class TestModel;

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
	/// <param name=""></param>
	SceneGame(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~SceneGame(void);

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name=""></param>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

private:

	// 状態
	STATE state_;

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// 状態別更新処理管理
	std::unordered_map<STATE, std::function<void()>> stateUpdateMap_;

	// 状態別描画処理管理
	std::unordered_map<STATE, std::function<void()>> stateDrawMap_;

	// テスト
	std::unique_ptr<TestModel> test_;

	// 更新関数
	void NormalUpdate(void) override;

	// 描画関数
	void NormalDraw(void) override;

	// 処理の変更
	void ChangeNormal(void) override;

	// 処理の登録
	void RegisterStateFunction(const STATE state, std::function<void()> update, std::function<void()> draw);

	// 状態別更新処理
	void UpdatePlay();
	void UpdateReporting();

	// 状態別描画処理
	void DrawPlay();
	void DrawReporting();
	
	//デバッグ処理
	void DebugUpdate(void);
	void DebugDraw(void);
};