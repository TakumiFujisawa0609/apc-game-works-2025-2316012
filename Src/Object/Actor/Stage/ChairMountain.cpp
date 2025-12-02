#include "ChairMountain.h"

ChairMountain::ChairMountain(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
	Load();
	Init();
	SetAnomaly();
}

ChairMountain::~ChairMountain()
{
}

void ChairMountain::Refresh()
{
	StageGimmickObjectBase::Refresh();

	// íœ“o˜^
	isDelete_ = true;
}
