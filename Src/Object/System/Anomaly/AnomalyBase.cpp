#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../Actor/Character/Player.h"
#include "AnomalyBase.h"

AnomalyBase::AnomalyBase():
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	resMng_(ResourceManager::GetInstance())
{
	player_ = nullptr;
}

AnomalyBase::~AnomalyBase()
{
}

void AnomalyBase::Load()
{
	player_ = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));
}

void AnomalyBase::Init()
{
}

void AnomalyBase::Occurrence(Json& param)
{
	// プレイヤーの狂気更新値を上昇させる
	player_->AddMadnessUpdateStep();
}