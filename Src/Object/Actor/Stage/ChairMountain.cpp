#include "ChairMountain.h"

ChairMountain::ChairMountain(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
}

ChairMountain::~ChairMountain()
{
}

void ChairMountain::Refresh()
{
	// íœ“o˜^
	isDelete_ = true;
}
