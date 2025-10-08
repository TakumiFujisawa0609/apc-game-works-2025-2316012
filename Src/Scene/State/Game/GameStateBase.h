#pragma once

class SceneGame;
class StageManager;
class CharacterManager;
class GameSystemManager;

class GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親のインスタンス</param>
	GameStateBase(SceneGame& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameStateBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

protected:

	// 親クラスの参照インスタンス
	SceneGame& parent_;

	// 各管理クラスの参照
	StageManager& stageMng_;
	CharacterManager& charaMng_;
	GameSystemManager& systemMng_;

	// 状態遷移の確認処理
	virtual void CheckChangeState() = 0;

};

