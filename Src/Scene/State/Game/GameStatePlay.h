#pragma once
#include "GameStateBase.h"

class CollisionManager;
class AnomalyManager;

class GameStatePlay : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStatePlay();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStatePlay() override;

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
	AnomalyManager& anomalyMng_;
};

