#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "ControllerCameraBase.h"

ControllerCameraBase::ControllerCameraBase() :
	camera_(mainCamera)
{
	step_ = 0.0f;
	transitionTime_ = 0.0f;
	isEnd_ = false;
	startCameraPos_ = Utility3D::VECTOR_ZERO;
	startTargetPos_ = Utility3D::VECTOR_ZERO;
	goalCameraPos_ = Utility3D::VECTOR_ZERO;
	goalTargetPos_ = Utility3D::VECTOR_ZERO;
}

ControllerCameraBase::~ControllerCameraBase()
{
}

void ControllerCameraBase::Init()
{
	step_ = 0.0f;
	transitionTime_ = 0.0f;
	isEnd_ = false;
	startCameraPos_ = Utility3D::VECTOR_ZERO;
	startTargetPos_ = Utility3D::VECTOR_ZERO;
	goalCameraPos_ = Utility3D::VECTOR_ZERO;
	goalTargetPos_ = Utility3D::VECTOR_ZERO;
}

void ControllerCameraBase::Update()
{
}

void ControllerCameraBase::Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const float transitionTime)
{
	// カメラを固定する
	camera_.ChangeMode(Camera::MODE::FIXED_POINT);

	// 時間の設定
	transitionTime_ = transitionTime;

	// 終了位置の設定
	goalCameraPos_ = goalCameraPos;
	goalTargetPos_ = goalTargetPos;

	// 開始位置の設定
	startCameraPos_ = camera_.GetPos();
	startTargetPos_ = camera_.GetTargetPos();

	// 初期化
	isEnd_ = false;
	step_ = 0.0f;
}