#include "../../Manager/Generic/GameStateManager.h"
#include "../../Manager/System/GameSystemManager.h"
#include "CoreGameBase.h"

CoreGameBase::CoreGameBase() :
	stateMng_(GameStateManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance())
{
}

CoreGameBase::~CoreGameBase()
{
}