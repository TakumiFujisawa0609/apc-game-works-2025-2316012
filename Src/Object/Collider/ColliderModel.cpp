#include "ColliderModel.h"

ColliderModel::ColliderModel(ActorBase& owner, const COLLISION_TAG tag) :
	ColliderBase(owner, tag)
{
	collResultPolyDim_ = {};
	collResultPoly_ = {};
	type_ = TYPE::MODEL;
}

ColliderModel::~ColliderModel()
{
}
