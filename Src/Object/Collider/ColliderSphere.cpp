#include "ColliderSphere.h"

ColliderSphere::ColliderSphere(ActorBase& owner, const CollisionTags::TAG tag) : 
	ColliderBase(owner, tag)
{
	type_ = ColliderType::TYPE::SPHERE;
	radius_ = 0.0f;
}

ColliderSphere::~ColliderSphere()
{
}
