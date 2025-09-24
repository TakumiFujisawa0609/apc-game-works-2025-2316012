#include "../ActorBase.h"
#include "ColliderBase.h"

ColliderBase::ColliderBase(ActorBase& owner, COLLISION_TAG tag) :
	owner_(owner),
	tag_(tag),
	transformOwner_(owner_.GetTransform())
{
	isHit_ = false;
	isDelete_ = false;
	type_ = TYPE::NONE;
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::OnHit(std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 所有者のインスタンスを渡す
	owner_.OnHit(opponentCollider);
}
