#include "../../../Manager/Generic/GameStateManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Core/Game/ReportSystem.h"
#include "ControllerOnHitReport.h"

ControllerOnHitReport::ControllerOnHitReport(Player& owner) :
	owner_(owner),
	systemMng_(GameSystemManager::GetInstance())
{
	report_ = nullptr;
	RegisterOnHit(CollisionTags::TAG::GHOST, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::STAGE_GIMMICK, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
}

ControllerOnHitReport::~ControllerOnHitReport()
{
}

void ControllerOnHitReport::Init()
{
	// レポートのポインタを保持
	report_ = dynamic_cast<ReportSystem*>(&GameSystemManager::GetInstance().GetGamsSystem(GameSystemManager::TYPE::REPORT));

	// キャストが失敗した場合、アサートを発動する
	assert(report_ != nullptr && "dynamic_castに失敗しました");
}

void ControllerOnHitReport::OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// レポートの状態遷移処理
	report_->ChangeReporting();

	// ゲーム状態変更
	stateMng_.ChangeState(GameStateManager::STATE::REPORTING);
}