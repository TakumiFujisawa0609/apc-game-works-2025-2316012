#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../Actor/Character/CharacterBase.h"
#include "ControllerActionBase.h"

ControllerActionBase::ControllerActionBase(CharacterBase& owner) :
	owner_(owner)
{
}

ControllerActionBase::~ControllerActionBase()
{
}