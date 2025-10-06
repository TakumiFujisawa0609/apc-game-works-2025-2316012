#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "GameStateBase.h"

GameStateBase::GameStateBase() :
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance())
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
