#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../Actor/Character/CharacterBase.h"
#include "ControllerActionBase.h"

ControllerActionBase::ControllerActionBase(CharacterBase& owner) :
	owner_(owner),
	collMng_(CollisionManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
}

ControllerActionBase::~ControllerActionBase()
{
}