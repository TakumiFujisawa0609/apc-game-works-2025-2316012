#include "Carpet.h"

Carpet::Carpet(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
	isTrans_ = true;
}

Carpet::~Carpet()
{
}
