#include "ColliderSphere.h"

ColliderSphere::ColliderSphere(ActorBase& owner, const CollisionTags::TAG tag) : 
	ColliderBase(owner, tag)
{
}

ColliderSphere::~ColliderSphere()
{
}
