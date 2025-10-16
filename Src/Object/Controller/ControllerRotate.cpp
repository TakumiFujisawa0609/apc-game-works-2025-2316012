#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
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
	ownerQuaRot_ = Quaternion::Slerp(ownerQuaRot_, owner_.GetGoalQuaRot(), owner_.GetStepRotTime());

	// ‰ñ“]‚Ì“K—p
	owner_.SetQuaRot(ownerQuaRot_);
}