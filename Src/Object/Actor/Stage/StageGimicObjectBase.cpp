#include "StageGimicObjectBase.h"

StageGimicObjectBase::StageGimicObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
}

StageGimicObjectBase::~StageGimicObjectBase()
{
}

void StageGimicObjectBase::SetChangeColor(const int red, const int green, const int blue)
{
}
