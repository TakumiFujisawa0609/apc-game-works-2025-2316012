#include "../../../Manager/Generic/StageManager.h"
#include "../../Actor/Stage/ChairMountain.h"
#include "AnomalyChairMountain.h"

AnomalyChairMountain::AnomalyChairMountain()
{
}

AnomalyChairMountain::~AnomalyChairMountain()
{
}

void AnomalyChairMountain::Load()
{
}

void AnomalyChairMountain::Init()
{
}

void AnomalyChairMountain::Occurrence(Json& param)
{
	// ê∂ê¨
	auto object = std::make_unique<ChairMountain>(KEY, param, stageMng_.GetStageObjectColliderParam(KEY));

	// í«â¡
	stageMng_.Add(KEY, std::move(object));
}
