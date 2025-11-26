#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/AnomalyManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Core/Game/ReportSystem.h"
#include "GameStatePlay.h"

GameStatePlay::GameStatePlay() :
	collMng_(CollisionManager::GetInstance()),
	anomalyMng_(AnomalyManager::GetInstance())
{
}

GameStatePlay::~GameStatePlay()
{
}

void GameStatePlay::Update()
{
	// 異変関係の処理
	anomalyMng_.Update();

	// キャラクターの本体更新
	charaMng_.Update();

	// ステージ更新
	stageMng_.Update();

	// システムの更新
	systemMng_.Update();

	// 衝突判定の更新
	collMng_.Update();

	// コライダーの削除
	collMng_.Sweep();

	// キャラクターの削除
	charaMng_.Sweep();

	// ステージの削除
	stageMng_.Sweep();
}

void GameStatePlay::Draw()
{
	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();

	// ポストエフェクトの描画
	effectMng_.Draw();

	// システム描画
	systemMng_.Draw();
}