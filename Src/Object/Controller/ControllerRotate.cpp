#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/Camera.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "../Actor/Character/CharacterBase.h"
#include "ControllerRotate.h"

ControllerRotate::ControllerRotate(CharacterBase& owner) : 
	owner_(owner)
{
	ownerQuaRot_ = Quaternion();
}

ControllerRotate::~ControllerRotate()
{
}

void ControllerRotate::Init()
{
	//‰Šú‰»
	ownerQuaRot_ = owner_.GetTransform().quaRot;
}

void ControllerRotate::Update()
{
	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	Quaternion quaRot = Quaternion::Slerp(owner_.GetTransform().quaRot, owner_.GetGoalQuaRot(), owner_.GetStepRotTime());

	// ‰ñ“]‚Ì“K—p
	owner_.SetQuaRot(quaRot);
}