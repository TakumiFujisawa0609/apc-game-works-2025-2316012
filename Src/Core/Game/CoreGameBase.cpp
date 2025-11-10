#include "../../Manager/Generic/GameStateManager.h"
#include "../../Manager/Resource/SoundManager.h"
#include "../../Manager/System/GameSystemManager.h"
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