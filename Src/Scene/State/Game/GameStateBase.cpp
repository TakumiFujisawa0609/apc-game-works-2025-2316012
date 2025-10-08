#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Core/CoreBase.h"
#include "GameStateBase.h"

GameStateBase::GameStateBase(SceneGame& parent) :
	parent_(parent),
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance())
{
}

GameStateBase::~GameStateBase()
{
}