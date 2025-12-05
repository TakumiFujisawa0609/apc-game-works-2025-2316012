#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../Actor/Character/Player.h"
#include "AnomalyBase.h"

AnomalyBase::AnomalyBase(const Json& param) :
	MADNESS_VALUE(param["madnessValue"]),
	scnMng_(SceneManager::GetInstance()),
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

void AnomalyBase::Init()
{
}

void AnomalyBase::Occurrence()
{	
	// プレイヤーの狂気更新値を上昇させる
	const auto & player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));
	player->AddMadnessValue(MADNESS_VALUE);


}

void AnomalyBase::Update()
{
}
