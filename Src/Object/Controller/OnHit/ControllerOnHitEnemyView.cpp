#include "../../Actor/Character/Enemy.h"
#include "../Action/ControllerActionEnemy.h"
#include "ControllerOnHitEnemyView.h"

ControllerOnHitEnemyView::ControllerOnHitEnemyView(Enemy& owner) :
	owner_(owner),
	actionCtrl_(*owner.GetActionEnemy())
{
	// 衝突物別処理の登録
	RegisterOnHit(CollisionTags::TAG::MAIN_STAGE, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::STAGE_GIMMICK, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
}

ControllerOnHitEnemyView::~ControllerOnHitEnemyView()
{
}

void ControllerOnHitEnemyView::OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// レイキャストによる遮蔽判定で衝突している場合
	// 敵の視野外なので視野範囲外と設定
	actionCtrl_.SetIsViewRange(false);
}