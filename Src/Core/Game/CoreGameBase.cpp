#include "../../Manager/Common/SoundManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Game/GameSystemManager.h"
#include "CoreGameBase.h"

CoreGameBase::CoreGameBase() :
	sndMng_(SoundManager::GetInstance()),
	inputMng_(InputManager::GetInstance()),
	stateMng_(GameStateManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance())
{
	// 初期状態はアクティブに
	isActive_ = true;
}

CoreGameBase::~CoreGameBase()
{
}