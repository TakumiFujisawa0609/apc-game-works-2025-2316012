#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../Actor/Character/Player.h"
#include "AnomalyBase.h"

AnomalyBase::AnomalyBase():
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
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
	// プレイヤーの狂気更新値を上昇させる
	const auto & player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));
	player->AddMadnessValue(10);
}