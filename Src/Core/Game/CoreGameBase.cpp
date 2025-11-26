#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Manager/Game/GameSystemManager.h"
#include "CoreGameBase.h"

CoreGameBase::CoreGameBase() :
	sndMng_(SoundManager::GetInstance()),
	stateMng_(GameStateManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance())
{
}

CoreGameBase::~CoreGameBase()
{
}