#include "../../Actor/Character/Ghost.h"
#include "ControllerOnHitGhost.h"

ControllerOnHitGhost::ControllerOnHitGhost(Ghost& owner) :
	owner_(owner)
{
	RegisterOnHit(CollisionTags::TAG::REPORT, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitRepot(opponentCollider); });
}

ControllerOnHitGhost::~ControllerOnHitGhost()
{
}

void ControllerOnHitGhost::OnHitRepot(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// ÉSÅ[ÉXÉgÇçÌèú
	owner_.SetDelete();
}
