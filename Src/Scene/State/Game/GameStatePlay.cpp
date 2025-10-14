#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Manager/System/AnomalyManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Core/Game/Report/ReportSystem.h"
#include "GameStatePlay.h"

GameStatePlay::GameStatePlay(SceneGame& parent) :
	GameStateBase(parent),
	collMng_(CollisionManager::GetInstance()),
	anomalyMng_(AnomalyManager::GetInstance())
{
	report_ = nullptr;
}

GameStatePlay::~GameStatePlay()
{
}

void GameStatePlay::Init()
{
	// レポートのポインタを保持
	report_ = dynamic_cast<ReportSystem*>(&systemMng_.GetGamsSystem(GameSystemManager::TYPE::REPORT));

	// キャストが失敗した場合、アサートを発動する
	assert(report_ != nullptr && "dynamic_castに失敗しました");
}

void GameStatePlay::Update()
{
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
}

void GameStatePlay::Draw()
{
	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();

	// システム描画
	systemMng_.Draw();
}

void GameStatePlay::CheckChangeState()
{
	// 報告中の場合
	if (report_->GetState() == ReportSystem::STATE::REPORTING)
	{
		// 状態変更
		parent_.ChangeState(SceneGame::STATE::REPORTING);
	}
}
