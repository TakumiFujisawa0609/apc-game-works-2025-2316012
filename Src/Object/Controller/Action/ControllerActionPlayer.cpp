#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/System/GameSystemManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Core/Game/Message.h"
#include "../../Actor/Character/Player.h"
#include "../../Collider/ColliderLine.h"
#include "../ControllerAnimation.h"
#include "ControllerActionPlayer.h"

ControllerActionPlayer::ControllerActionPlayer(Player& player) :
	ControllerActionBase(player),
	player_(player),
	input_(InputManager::GetInstance()),
	REPORT_INPUT_TIME(player.GetReportTime()),
	TIME_ROT(player.GetTimeRot())
{
	stepRotTime_ = 0.0f;
	madnessStep_ = 0.0f;
	isEndLanding_ = false;
	isWarningMadness_ = false;
}

ControllerActionPlayer::~ControllerActionPlayer()
{
}

void ControllerActionPlayer::Init()
{
	// 各種変数の初期化
	isWarningMadness_ = false;
	isEndLanding_ = false;
}

void ControllerActionPlayer::Update()
{
	// 移動操作処理
	ProcessMove();

	// レポート操作処理
	ProcessReport();

	// 狂気値の警告
	WarningMadness();
}

void ControllerActionPlayer::DebugDraw()
{
	// 画面中心から座標を取得
	VECTOR screenCenter = ConvScreenPosToWorldPos({ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y, 0 });

	// 末端の位置を取得
	VECTOR endPos = VAdd(screenCenter, VScale(mainCamera.GetForward(), REPORT_RANGE));

	// 先端位置を取得
	VECTOR startPos = player_.GetTransform().pos;
	startPos.y += OFFSET_Y;

	DrawLine3D(startPos, endPos, UtilityCommon::BLUE);
}

void ControllerActionPlayer::ProcessMove()
{
	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();

	// プレイヤーのジャンプ判定
	bool isJump = player_.IsJump();

	// 回転したい角度
	double rotDeg = 0;

	// 方向ベクトル
	VECTOR dir = Utility3D::VECTOR_ZERO;

	// 移送速度
	float speed = 0.0f;

	// 右移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_RIGHT))
	{
		rotDeg = ROT_DEG_RIGHT;
		dir = cameraRot.GetRight();
	}

	// 左移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_LEFT))
	{
		rotDeg = ROT_DEG_LEFT;
		dir = cameraRot.GetLeft();
	}

	// 前移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_UP))
	{
		rotDeg = ROT_DEG_FRONT;
		dir = cameraRot.GetForward();
	}

	// 後移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_DOWN))
	{
		rotDeg = ROT_DEG_BACK;
		dir = cameraRot.GetBack();
	}

	// プレイヤーの着地判定
	isEndLanding_ = IsEndLanding();

	// アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();	
	
	// ジャンプ中じゃないかつ操作入力がされたとき
	if (!Utility3D::EqualsVZero(dir) && (isJump || isEndLanding_))
	{
		// 回転処理
		double rotRad = UtilityCommon::Deg2RadD(rotDeg);
		SetGoalRotate(rotRad);

		// ダッシュ
		bool isDash = input_.IsNew(InputManager::TYPE::PLAYER_DASH);

		// 歩く移動速度取得
		speed = player_.GetSpeedMove();

		// ステップを更新
		madnessStep_ += player_.GetMadnessUpdateStep();

		// ステップが最大に達したら
		if (madnessStep_ > 1.0f)
		{
			// 狂気値追加
			player_.AddMadnessValue(MADNESS_ADD_VALUE);

			// ステップを初期化
			madnessStep_ = 0.0f;
		}

		// ダッシュ中かつ狂気値が一定未満の場合
		if (isDash && MADNSEE_CONDITION > player_.GetMadnessValue())
		{
			// 速度を変更
			speed = player_.GetSpeedRun();
		}

		if (!isJump && isEndLanding_)
		{
			// アニメーション
			if (isDash)
			{
				animation.Play(Player::ANIM_RUN);
			}
			else
			{
				animation.Play(Player::ANIM_WALK);
			}
		}
	}
	else
	{
		if (!isJump && IsEndLanding())
		{
			animation.Play(Player::ANIM_IDLE);
		}
	}

	// 移動ベクトルの設定
	player_.SetMoveDir(dir);

	// 移動量の設定
	player_.SetMoveSpeed(GetApplyMadnessToSpeed(speed));

	// 回転用ステップを更新
	stepRotTime_ -= scnMng_.GetDeltaTime();
}

void ControllerActionPlayer::ProcessJump()
{
	// 現在のジャンプ判定
	bool isJump = player_.IsJump();

	// 現在のジャンプ用ステップ
	float stepJump = player_.GetStepJump();

	// ジャンプ受付時間
	const float JUMP_ACCEPT_TIME = player_.GetJumpAcceptTime();

	// ジャンプキーを入力したか
	bool isInputJump = input_.IsNew(InputManager::TYPE::PLAYER_JUMP);

	// アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// ジャンプ
	if (isInputJump && (isJump || isEndLanding_))
	{
		// ジャンプ中ではない場合
		if (!isJump)
		{
			animation.Play(Player::ANIM_JUMP, true, 13.0f, 25.0f);
			animation.SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		// ジャンプ中に設定
		isJump = true;

		// ジャンプの入力受付時間をヘラス
		stepJump += scnMng_.GetDeltaTime();
		if (stepJump < JUMP_ACCEPT_TIME)
		{
			player_.SetJumpPow(VScale(Utility3D::DIR_U, player_.GetJumpAmount()));
		}
	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isInputJump)
	{
		stepJump = JUMP_ACCEPT_TIME;
	}

	// ジャンプ判定の設定
	player_.SetIsJump(isJump);

	// ステップの設定
	player_.SetStepJump(stepJump);
}

void ControllerActionPlayer::ProcessReport()
{
	// 初期パーセント(未入力の場合は0)
	float newPercent = 0;

	// 入力中の場合
	if(input_.IsNew(InputManager::TYPE::ANOMARY_REPORT))
	{		
		// レポート進捗率取得
		float nowPercent = player_.GetReportPercent();
		
		// ゲージの更新値を計算
		float step = GAUGE_MAX / REPORT_INPUT_TIME * scnMng_.GetDeltaTime();

		// 新しい進捗率を計算
		newPercent = nowPercent + step;
	}

	// 進捗率が最大値以上の場合
	if (GAUGE_MAX <= newPercent)
	{
		// 線の判定を生成
		CreateLineCollider();
	}

	// 設定
	player_.SetReportPercent(newPercent);
}

bool ControllerActionPlayer::IsEndLanding() const
{
	//アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// アニメーションがジャンプではない
	if (animation.GetPlayType() != Player::ANIM_JUMP)
	{
		return true;
	}

	// アニメーションが終了しているか
	if (animation.IsEnd())
	{
		return true;
	}

	return false;
}

void ControllerActionPlayer::SetGoalRotate(const double rotRad)
{
	// 目標回転角度
	Quaternion goalQuaRot = player_.GetGoalQuaRot();	

	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, Utility3D::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot = axis;

	// 設定
	player_.SetStepRotTime((TIME_ROT - stepRotTime_) / TIME_ROT);
	player_.SetGoalQuaRot(goalQuaRot);
}

void ControllerActionPlayer::CreateLineCollider()
{
	//コライダー生成
	std::shared_ptr<ColliderLine> coll = std::make_shared<ColliderLine>(player_, CollisionTags::TAG::REPORT);

	// 画面中心から座標を取得
	VECTOR screenCenter = ConvScreenPosToWorldPos({ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y, 0 });

	// 末端の位置を取得
	VECTOR endPos = VAdd(screenCenter, VScale(mainCamera.GetForward(), REPORT_RANGE));

	// 先端位置を取得
	VECTOR startPos = player_.GetTransform().pos;
	startPos.y += OFFSET_Y;

	// 先端位置設定
	coll->SetLocalPosPointHead(startPos);

	// 末端位置の設定
	coll->SetLocalPosPointEnd(endPos);

	// 判定後すぐ消す
	coll->SetDelete();

	// 判定に追加
	collMng_.Add(std::move(coll));
}

void ControllerActionPlayer::WarningMadness()
{
	// 狂気値が一定未満の場合
	if (player_.GetMadnessValue() < MADNSEE_CONDITION)
	{
		// 判定を無効にして受付状態にする
		isWarningMadness_ = false;
		return;
	}
	// または警告表示中の場合
	else if (isWarningMadness_)
	{
		// 無視
		return;
	}

	// メッセージを表示 
	GameSystemManager::GetInstance().ChangeMessage(Message::TYPE::MADNESS);

	// 判定を有効
	isWarningMadness_ = true;
}

const float ControllerActionPlayer::GetApplyMadnessToSpeed(float speed)
{
	constexpr int MAX = 50;
	constexpr float PERCENTAGE = 100.0f;

	// 狂気値を取得
	int value = player_.GetMadnessValue();

	// 減少率
	float rate = MAX / GAUGE_MAX * value / PERCENTAGE;

	//乗算
	return speed * (1.0f - rate);
}
