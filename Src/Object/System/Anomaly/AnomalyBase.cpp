#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "AnomalyBase.h"

AnomalyBase::AnomalyBase():
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	resMng_(ResourceManager::GetInstance())
{
}

AnomalyBase::~AnomalyBase()
{
}

void AnomalyBase::Load()
{
}

void AnomalyBase::Init()
{
}

void AnomalyBase::Occurrence(Json& param)
{
}