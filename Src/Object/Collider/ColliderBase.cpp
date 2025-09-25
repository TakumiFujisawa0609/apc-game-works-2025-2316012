#include "../ActorBase.h"
#include "ColliderBase.h"

ColliderBase::ColliderBase(ActorBase& owner, COLLISION_TAG tag) :
	owner_(owner),
	tag_(tag),
	transformOwner_(owner_.GetTransform())
{
	isHit_ = false;
	isDelete_ = false;
	type_ = TYPE::MAX;
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::DebugDraw()
{
}

void ColliderBase::OnHit(std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 所有者のインスタンスを渡す
	owner_.OnHit(opponentCollider);
}

VECTOR ColliderBase::GetRotPos(const VECTOR& localPos) const
{
	VECTOR localRotPos = transformOwner_.quaRot.PosAxis(localPos);
	return VAdd(transformOwner_.pos, localRotPos);
}