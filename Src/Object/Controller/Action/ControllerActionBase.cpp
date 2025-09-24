#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "../../Character/CharacterBase.h"
#include "ControllerActionBase.h"

ControllerActionBase::ControllerActionBase(CharacterBase& owner) :
	owner_(owner)
{
}

ControllerActionBase::~ControllerActionBase()
{
}