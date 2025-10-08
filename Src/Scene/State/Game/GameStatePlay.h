#pragma once
#include "GameStateBase.h"

class CollisionManager;

class GameStatePlay : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親のインスタンス</param>
	GameStatePlay(SceneGame& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStatePlay() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 各種管理クラスの参照
	CollisionManager& collMng_;

	// レポート
	ReportSystem* report_;

	// 状態遷移確認
	void CheckChangeState() override;
};

