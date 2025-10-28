#include "ChairMountain.h"

ChairMountain::ChairMountain(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
	Load();
	Init();
	isActive_ = true;
}

ChairMountain::~ChairMountain()
{
}

void ChairMountain::Refresh()
{
	// íœ“o˜^
	isDelete_ = true;
}
