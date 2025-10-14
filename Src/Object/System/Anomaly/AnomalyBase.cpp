#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../Actor/Character/CharacterBase.h"
#include "AnomalyBase.h"

AnomalyBase::AnomalyBase():
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance())
{
}

AnomalyBase::~AnomalyBase()
{
}

void AnomalyBase::Init()
{

}

void AnomalyBase::Update()
{

}

void AnomalyBase::Draw()
{

}

void AnomalyBase::Occurrence(Json& param)
{
}