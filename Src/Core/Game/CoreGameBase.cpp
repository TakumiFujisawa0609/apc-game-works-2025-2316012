#include "../../Manager/Generic/GameStateManager.h"
#include "CoreGameBase.h"

CoreGameBase::CoreGameBase() :
	stateMng_(GameStateManager::GetInstance())
{
}

CoreGameBase::~CoreGameBase()
{
}