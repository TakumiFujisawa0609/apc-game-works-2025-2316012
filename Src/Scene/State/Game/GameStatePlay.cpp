#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "GameStatePlay.h"

GameStatePlay::GameStatePlay() :
	collMng_(CollisionManager::GetInstance())
{
}

GameStatePlay::~GameStatePlay()
{
}

void GameStatePlay::Init()
{
}

void GameStatePlay::Update()
{
	// キャラクターの本体更新
	CharacterManager::GetInstance().Update();

	// ステージ更新
	StageManager::GetInstance().Update();

	// 衝突判定の更新
	CollisionManager::GetInstance().Update();

	// コライダーの削除
	CollisionManager::GetInstance().Sweep();
}

void GameStatePlay::Draw()
{
	// ステージ描画
	StageManager::GetInstance().Draw();

	// キャラクター描画
	CharacterManager::GetInstance().Draw();
}
