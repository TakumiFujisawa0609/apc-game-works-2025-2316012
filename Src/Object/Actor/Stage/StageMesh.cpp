#include "StageMesh.h"
#include "../../Collider/ColliderBase.h"

StageMesh::StageMesh(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
}

StageMesh::~StageMesh()
{
}

void StageMesh::SetDelete()
{
	isDelete_ = true;
	collider_->SetDelete();
}