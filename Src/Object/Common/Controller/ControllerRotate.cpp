#include "../../../Manager/Generic/SceneManager.h"
#include "ControllerRotate.h"

ControllerRotate::ControllerRotate()
{
}

ControllerRotate::~ControllerRotate()
{
}

void ControllerRotate::Rotate()
{

}

void ControllerRotate::SetGoalRotate(double rotRad)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	rotY_ = Quaternion::Slerp(rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ControllerRotate::SetRotate(Quaternion& targetQuaternion)
{
	targetQuaternion = rotY_;
}