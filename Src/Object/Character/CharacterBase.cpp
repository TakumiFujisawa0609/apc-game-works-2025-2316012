#include <DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../Common/Quaternion.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(const Json& param) :
	SPEED_MOVE(param["moveSpeed"]),
	SPEED_RUN(param["dashSpeed"]),
	GRAVITY(param["gravity"]),
	TIME_ROT(param["timeRot"]),
	ANIM_DEFAULT_SPEED(param["animationDefaultSpeed"]),
	INITIAL_POS({ param["initialPosition"]["x"],param["initialPosition"]["y"],param["initialPosition"]["z"] })
{
	stepRotTime_ = 0.0f;
	movePower_ = Utility3D::VECTOR_ZERO;
	moveDir_ = Utility3D::VECTOR_ZERO;
	rotY_ = Quaternion();
}

void CharacterBase::Init()
{	
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y), 0.0f });
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.Update();
}

void CharacterBase::UpdateApply()
{
	transform_.Update();
}

void CharacterBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}

void CharacterBase::SetGoalRotate(double rotRad)
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

void CharacterBase::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	rotY_ = Quaternion::Slerp(rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}