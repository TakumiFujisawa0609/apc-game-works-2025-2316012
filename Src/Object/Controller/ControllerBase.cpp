#include "../../Manager/Generic/SceneManager.h"
#include "ControllerBase.h"

ControllerBase::ControllerBase() : 
	scnMng_(SceneManager::GetInstance())
{
}

ControllerBase::~ControllerBase()
{
}

void ControllerBase::Update()
{
}