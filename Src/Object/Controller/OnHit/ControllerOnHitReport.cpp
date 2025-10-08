#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Core/Game/Report/ReportSystem.h"
#include "ControllerOnHitReport.h"

ControllerOnHitReport::ControllerOnHitReport(Player& owner) :
	owner_(owner),
	systemMng_(GameSystemManager::GetInstance())
{
}

ControllerOnHitReport::~ControllerOnHitReport()
{

}

void ControllerOnHitReport::OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// レポートシステムの状態遷移
	auto report = std::dynamic_pointer_cast<ReportSystem>(systemMng_.GetGamsSystem(GameSystemManager::TYPE::REPORT));
	report.ChangeReporting();
}