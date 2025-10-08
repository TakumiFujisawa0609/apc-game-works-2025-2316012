#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Core/Game/Report/ReportSystem.h"
#include "GameStatePlay.h"

GameStatePlay::GameStatePlay(SceneGame& parent) :
	GameStateBase(parent),
	collMng_(CollisionManager::GetInstance())
{
	report_ = nullptr;
}

GameStatePlay::~GameStatePlay()
{
	report_ = nullptr;
	delete report_;
}

void GameStatePlay::Init()
{
	// レポートのポインタを保持
	report_ = dynamic_cast<ReportSystem*>(&systemMng_.GetGamsSystem(GameSystemManager::TYPE::REPORT));
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
