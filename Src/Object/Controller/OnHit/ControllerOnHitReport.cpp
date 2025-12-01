#include "../../../Manager/Game/GameStateManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Core/Game/ReportSystem.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Stage/StageGimmickObjectBase.h"
#include "../../../Object/Actor/Character/Player.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "ControllerOnHitReport.h"

ControllerOnHitReport::ControllerOnHitReport(Player& owner) :
	owner_(owner),
	systemMng_(GameSystemManager::GetInstance())
{
	report_ = nullptr;
	RegisterOnHit(CollisionTags::TAG::GHOST, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitGhost(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::ANOMALY, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitAnomaly(opponentCollider); });
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

void ControllerOnHitReport::OnHitAnomaly(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 衝突後処理を実行
	OnHitCommon();
}

void ControllerOnHitReport::OnHitGhost(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 共通処理
	OnHitCommon();
}

void ControllerOnHitReport::OnHitCommon()
{
	// レポートの状態遷移処理
	report_->ChangeReporting();

	// ゲーム状態変更
	stateMng_.ChangeState(GameStateManager::STATE::REPORTING);

	// 狂気値を減らす
	owner_.AddMadnessValue(-100);

	// 全ての効果音停止
	sndMng_.StopAllSe();

	// サウンド再生
	sndMng_.PlaySe(SoundType::SE::REPORTING);
}