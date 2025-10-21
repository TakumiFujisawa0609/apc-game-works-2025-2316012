#pragma once
#include <memory>
#include "SceneBase.h"

class GameStateManager;
class CharacterManager;
class StageManager;
class GameSystemManager;
class AnomalyManager;
class ScenePause;
class TestModel;
class CreatePositionList;

class SceneGame : public SceneBase
{
public:

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

private:

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

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