#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Core/Game/CoreGameBase.h"
#include "GameStateBase.h"

GameStateBase::GameStateBase() :
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance()),
	effectMng_(GameEffectManager::GetInstance())
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
