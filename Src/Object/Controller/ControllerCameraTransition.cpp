#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "ControllerCameraTransition.h"

ControllerCameraTransition::ControllerCameraTransition() :
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

ControllerCameraTransition::~ControllerCameraTransition()
{
}

void ControllerCameraTransition::Init()
{
	step_ = 0.0f;
	transitionTime_ = 0.0f;
	isEnd_ = false;
	startCameraPos_ = Utility3D::VECTOR_ZERO;
	startTargetPos_ = Utility3D::VECTOR_ZERO;
	goalCameraPos_ = Utility3D::VECTOR_ZERO;
	goalTargetPos_ = Utility3D::VECTOR_ZERO;
}

void ControllerCameraTransition::Update()
{
	// 終了済みの場合
	if (isEnd_)
	{
		// 処理を飛ばす
		return;
	}

	// ステップ更新
	step_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = step_ / transitionTime_;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
		isEnd_ = true;	// 終了判定を立てる
	}

	// 線形補間
	VECTOR currentPos = UtilityCommon::Lerp(startCameraPos_, goalCameraPos_, t);
	VECTOR currentTarget = UtilityCommon::Lerp(startTargetPos_, goalTargetPos_, t);

	// カメラの設定
	camera_.SetPos(currentPos);
	camera_.SetTargetPos(currentTarget);
}

void ControllerCameraTransition::Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const float transitionTime)
{
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