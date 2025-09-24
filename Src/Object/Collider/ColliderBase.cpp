#include "../ActorBase.h"
#include "ColliderBase.h"

ColliderBase::ColliderBase(ActorBase& owner) :
	owner_(owner)
{
	isHit_ = false;
	isDelete_ = false;
	type_ = TYPE::NONE;
}

ColliderBase::~ColliderBase()
{
}