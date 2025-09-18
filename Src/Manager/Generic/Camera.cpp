#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <algorithm>
#include "../../Application.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility//UtilityCommon.h"
#include "../../Object/Common/Transform.h"
#include "InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility3D::VECTOR_ZERO;
	targetPos_ = Utility3D::VECTOR_ZERO;
	localF2CPos_ = Utility3D::VECTOR_ZERO;
	localF2TPos_ = Utility3D::VECTOR_ZERO;
	followTransform_ = nullptr;

	//状態遷移処理の登録
	changeModeMap_.emplace(MODE::NONE, std::bind(&Camera::ChangeModeNone, this));
	changeModeMap_.emplace(MODE::FIXED_POINT, std::bind(&Camera::ChangeModeFixedPoint, this));
	changeModeMap_.emplace(MODE::FOLLOW, std::bind(&Camera::ChangeModeFollow, this));
	changeModeMap_.emplace(MODE::FPS, std::bind(&Camera::ChangeModeFps, this));
	changeModeMap_.emplace(MODE::FREE, std::bind(&Camera::ChangeModeFree, this));
}

void Camera::Init(void)
{
	// カメラの初期設定
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	// カメラモードごとの描画前処理
	beforeDrawFunc_();

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

Camera::MODE Camera::GetMode() const
{
	return mode_;
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 状態遷移処理の実行
	changeModeMap_[mode_]();
}

void Camera::SetDefault(void)
{
	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility3D::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility3D::DIR_U;

	angles_.x = UtilityCommon::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();
}

void Camera::SyncFollow(void)
{
	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility3D::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(localF2TPos_);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(localF2CPos_);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = Utility3D::DIR_U;
}

void Camera::ProcessRotFollow(void)
{
	auto& ins = InputManager::GetInstance();
	float rotPow = UtilityCommon::Deg2RadF(SPEED);
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }

	if (angles_.x >= LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x <= LIMIT_X_DW_RAD)
	{
		angles_.x = LIMIT_X_DW_RAD;
	}
}

void Camera::ProcessRotFps(void)
{
	auto& ins = InputManager::GetInstance();
	const float fovPer = 0.5f; // 視野角に対するマウス移動量の割合

	//マウス座標を取得
	int mousePosX = ins.GetMousePos().x;
	int mousePosY = ins.GetMousePos().y;

	//マウスの移動量をクランプして、カメラの角度に反映する
	mousePosY += float(std::clamp(mousePosY - Application::SCREEN_SIZE_X / 2, -120, 120)) * fovPer / GetFPS();
	mousePosX += float(std::clamp(mousePosX - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fovPer / GetFPS();

	// マウスの位置を画面中央に戻す
	SetMousePoint(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y);

	//角度制限
	if (angles_.x <= LIMIT_X_UP_RAD_FPS)
	{
		angles_.x = LIMIT_X_UP_RAD_FPS;
	}
	else if (angles_.x >= LIMIT_X_DW_RAD_FPS)
	{
		angles_.x = LIMIT_X_DW_RAD_FPS;
	}
}

void Camera::ProcessRotFree()
{
	// 任意の定数
	const float CAMERA_MOVE_SPEED = 4.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;
	const float CAMERA_DISTANCE = 50.0f; // 注視点からカメラまでの距離

	// キーボード入力による回転
	if (CheckHitKey(KEY_INPUT_RIGHT)) angles_.y += CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))  angles_.y -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_UP))    angles_.x -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_DOWN))  angles_.x += CAMERA_ROTATE_SPEED;

	// 角度の制限
	if (angles_.x <= LIMIT_X_UP_RAD_FPS) { angles_.x = LIMIT_X_UP_RAD_FPS; }
	else if (angles_.x >= LIMIT_X_DW_RAD_FPS) { angles_.x = LIMIT_X_DW_RAD_FPS; }

	// キーボード入力による注視点の移動
	if (CheckHitKey(KEY_INPUT_W)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_S)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_A)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_D)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetRight(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_Q)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetDown(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_E)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetUp(), CAMERA_MOVE_SPEED));

	//角度を計算
	rot_ = (Quaternion::Quaternion(angles_));

	// 注視点からカメラ位置までの相対座標を計算
	VECTOR localPos = rot_.PosAxis(LOCAL_F2T_POS_FOLLOW);

	// 注視点を更新
	targetPos_ = VAdd(pos_, localPos);
}

void Camera::ChangeModeNone()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawNone, this);
}

void Camera::ChangeModeFixedPoint()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawNone, this);
}

void Camera::ChangeModeFollow()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFollow, this);

	//相対座標の設定
	localF2CPos_ = LOCAL_F2C_POS_FOLLOW;
	localF2TPos_ = LOCAL_F2T_POS_FOLLOW;
}

void Camera::ChangeModeFps()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFps, this);

	//相対座標の設定
	localF2CPos_ = LOCAL_F2C_POS_FPS;
	localF2TPos_ = LOCAL_F2T_POS_FPS;
}

void Camera::ChangeModeFree()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFree, this);
}

void Camera::SetBeforeDrawFollow(void)
{
	// カメラ操作
	ProcessRotFollow();

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFps()
{
	//マウスでのカメラ操作
	ProcessRotFps();

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFree()
{
	//カメラ操作
	ProcessRotFree();
}
