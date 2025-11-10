#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Resource/SoundManager.h"
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