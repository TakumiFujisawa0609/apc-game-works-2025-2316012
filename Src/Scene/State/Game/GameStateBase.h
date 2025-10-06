#pragma once

class StageManager;
class CharacterManager;

class GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStateBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameStateBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected:

	// 各管理クラスの参照
	StageManager& stageMng_;
	CharacterManager& charaMng_;
};

