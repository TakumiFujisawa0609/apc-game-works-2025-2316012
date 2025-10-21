#include <cassert>
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Core/Game/Report/ReportSystem.h"
#include "GameStateReporting.h"

GameStateReporting::GameStateReporting()
{
	report_ = nullptr;
}

GameStateReporting::~GameStateReporting()
{
}

void GameStateReporting::Init()
{
	// レポートのポインタを保持
	report_ = dynamic_cast<ReportSystem*>(&systemMng_.GetGamsSystem(GameSystemManager::TYPE::REPORT));

	// キャストが失敗した場合、アサートを発動する
	assert(report_ != nullptr && "dynamic_castに失敗しました");
}

void GameStateReporting::Update()
{
	// レポートのみ更新
	report_->Update();
}

void GameStateReporting::Draw()
{
	// レポートのみ描画
	report_->Draw();
}