#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Core/Game/CoreGameBase.h"
#include "GameStateBase.h"

GameStateBase::GameStateBase() :
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance())
{
}

GameStateBase::~GameStateBase()
{
}

void GameStateBase::Init()
{
}

void GameStateBase::Update()
{
}

void GameStateBase::Draw()
{
}
