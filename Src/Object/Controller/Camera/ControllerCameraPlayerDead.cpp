#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "ControllerCameraPlayerDead.h"

ControllerCameraPlayerDead::ControllerCameraPlayerDead()
{
}

ControllerCameraPlayerDead::~ControllerCameraPlayerDead()
{
}

void ControllerCameraPlayerDead::Update()
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
