#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../Actor/Character/Enemy.h"
#include "ControllerActionEnemyRespown.h"

ControllerActionEnemyRespown::ControllerActionEnemyRespown(Enemy& owner) : 
	ControllerActionBase(owner),
	owner_(owner),
	charaMng_(CharacterManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	systemMng_(GameSystemManager::GetInstance()),
	effectMng_(GameEffectManager::GetInstance())
{
	// èÛë‘ïœçXä÷êîä«óù
	changeStateMap_.emplace(STATE::NONE, std::bind(&ControllerActionEnemyRespown::UpdateNone, this));
	changeStateMap_.emplace(STATE::START, std::bind(&ControllerActionEnemyRespown::ChangeStateStart, this));
	changeStateMap_.emplace(STATE::WALK, std::bind(&ControllerActionEnemyRespown::ChangeStateWalk, this));
	changeStateMap_.emplace(STATE::MOVE_CAMERA_FORWARD, std::bind(&ControllerActionEnemyRespown::ChangeStateMoveCameraForward, this));
	changeStateMap_.emplace(STATE::ZOOM_OUT, std::bind(&ControllerActionEnemyRespown::ChangeStateZoomOut, this));
	changeStateMap_.emplace(STATE::ROAR, std::bind(&ControllerActionEnemyRespown::ChangeStateRoar, this));
	changeStateMap_.emplace(STATE::END, std::bind(&ControllerActionEnemyRespown::ChangeStateEnd, this));
}

ControllerActionEnemyRespown::~ControllerActionEnemyRespown()
{
}

void ControllerActionEnemyRespown::Init()
{
	// èâä˙èÛë‘Ç÷ïœçX
	ChangeState(STATE::NONE);
}

void ControllerActionEnemyRespown::Update()
{
	update_();
}

void ControllerActionEnemyRespown::DebugDraw()
{
}

void ControllerActionEnemyRespown::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state_]();
}

void ControllerActionEnemyRespown::ChangeStateNone()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateNone, this);
}

void ControllerActionEnemyRespown::ChangeStateStart()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateStart, this);
}

void ControllerActionEnemyRespown::ChangeStateWalk()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateWalk, this);
}

void ControllerActionEnemyRespown::ChangeStateMoveCameraForward()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateMoveCameraForward, this);
}

void ControllerActionEnemyRespown::ChangeStateZoomOut()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateZoomOut, this);
}

void ControllerActionEnemyRespown::ChangeStateRoar()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateRoar, this);
}

void ControllerActionEnemyRespown::ChangeStateEnd()
{
	update_ = std::bind(&ControllerActionEnemyRespown::UpdateEnd, this);
}

void ControllerActionEnemyRespown::UpdateStart()
{
}

void ControllerActionEnemyRespown::UpdateWalk()
{
}

void ControllerActionEnemyRespown::UpdateMoveCameraForward()
{
}

void ControllerActionEnemyRespown::UpdateZoomOut()
{
}

void ControllerActionEnemyRespown::UpdateRoar()
{
}

void ControllerActionEnemyRespown::UpdateEnd()
{
}
