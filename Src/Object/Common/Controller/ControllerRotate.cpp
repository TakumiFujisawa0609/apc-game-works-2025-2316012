#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "ControllerRotate.h"

ControllerRotate::ControllerRotate(const float timeRot) : 
	TIME_ROT(timeRot)
{
}

ControllerRotate::~ControllerRotate()
{
}

void ControllerRotate::Rotate()
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	rotY_ = Quaternion::Slerp(rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ControllerRotate::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, Utility3D::AXIS_Y);

	// Œ»ÝÝ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ðŽæ‚é
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// ‚µ‚«‚¢’l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void ControllerRotate::SetRotate(Quaternion& targetQuaternion)
{
	targetQuaternion = rotY_;
}