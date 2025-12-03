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

	GetCameraAngleTRotate();

	// カメラ回転処理
	//VECTOR currentUpVec = Quaternion::AngleAxis();

	// カメラの設定
	camera_.SetPos(currentPos);
	camera_.SetTargetPos(currentTarget);
	//camera_.SetCameraUpVector(currentUpVec);
}

void ControllerCameraBase::Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const VECTOR& goalAngles, const float transitionTime)
{	
	// カメラを固定する
	camera_.ChangeMode(Camera::MODE::FIXED_POINT);

	// 時間の設定
	transitionTime_ = transitionTime;

	// 終了位置の設定
	goalCameraPos_ = goalCameraPos;
	goalTargetPos_ = goalTargetPos;
	goalAngles_ = goalAngles;

	// 開始位置の設定
	startCameraPos_ = camera_.GetPos();
	startTargetPos_ = camera_.GetTargetPos();

	// 開始角度の計算


	// 初期化
	isEnd_ = false;
	step_ = 0.0f;
}

VECTOR ControllerCameraBase::GetStartAngles()
{
	// 1. **現在のカメラの向きから回転クォータニオンを逆算**

	// (A) ローカル座標系の3軸を取得 (View/Right/Up)
	VECTOR view = VNorm(VSub(startTargetPos_, startCameraPos_));
	VECTOR right = VNorm(VCross(view, mainCamera.GetCameraUpVector()));
	VECTOR up = VNorm(VCross(right, view));

	// (B) 回転行列の構築 (V_right, V_up, V_view から構成)
	// ここでは、行列の要素を手動で設定するか、ライブラリ関数を使用します。
	MATRIX rotaMat = MV1GetRotationMatrix(right, up, view); // 概念的な関数

	// (C) クォータニオンへ変換
	// ユーティリティクラスの Quaternions::FromRotationMatrix(RotationMatrix) を想定
	Quaternion Q_current = Quaternion::GetRotation(rotaMat);
}

void ControllerCameraBase::Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const float transitionTime)
{

}