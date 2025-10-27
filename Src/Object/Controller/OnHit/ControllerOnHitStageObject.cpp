#include "../../Actor/Stage/StageGimmickObjectBase.h"
#include "ControllerOnHitStageObject.h"

ControllerOnHitStageObject::ControllerOnHitStageObject(StageGimmickObjectBase& owner) :
	owner_(owner)
{
	RegisterOnHit(CollisionTags::TAG::REPORT, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitReport(opponentCollider); });
}

ControllerOnHitStageObject::~ControllerOnHitStageObject()
{
}

void ControllerOnHitStageObject::OnHitReport(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	owner_.Refresh();
}
