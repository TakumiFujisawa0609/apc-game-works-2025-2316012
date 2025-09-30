#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "../Actor/Character/CharacterBase.h"
#include "ControllerRotate.h"

ControllerRotate::ControllerRotate(CharacterBase& owner) : 
	owner_(owner),
	TIME_ROT(owner_.GetTimeRot())
{
	stepRotTime_ = 0.0f;
	goalQuaRot_ = Quaternion();
	myRotY_ = Quaternion();
}

ControllerRotate::~ControllerRotate()
{
}

void ControllerRotate::Init()
{
	//‰Šú‰»
	stepRotTime_ = 0.0f;
	goalQuaRot_ = Quaternion();
	myRotY_ = Quaternion();
}

void ControllerRotate::Update()
{
	//–Ú•W‰ñ“]Šp“x‚Ìİ’è
	SetGoalRotate();

	//‰ñ“]ˆ—
	Rotate();

	//“K—p
	Apply();
}

void ControllerRotate::SetGoalRotate()
{
	double rotDeg = owner_.GetRotDeg();

	//‰ñ“]Šp“x‚ªİ’è‚³‚ê‚Ä‚È‚¢‚Æ‚«
	if (rotDeg < 0.0f)
	{
		return;
	}

	//ƒ‰ƒWƒAƒ“•ÏŠ·
	double rotRad = UtilityCommon::Deg2RadD(rotDeg);

	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, Utility3D::AXIS_Y);

	// Œ»İİ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ğæ‚é
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// ‚µ‚«‚¢’l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void ControllerRotate::Rotate()
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	myRotY_ = Quaternion::Slerp(myRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ControllerRotate::Apply()
{
	owner_.SetRot(myRotY_);
}