#include "ControllerOnHitBase.h"

ControllerOnHitBase::ControllerOnHitBase()
{
}

ControllerOnHitBase::~ControllerOnHitBase()
{
}

void ControllerOnHitBase::Update()
{
}

void ControllerOnHitBase::RegisterOnHit(const std::string& tag, std::function<void()> onHit)
{
	onHitMap_[tag] = onHit;
}
