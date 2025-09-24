#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Character/Player.h"
#include "../../System/InputPlayer.h"
#include "../ControllerAnimation.h"
#include "ControllerActionPlayer.h"

ControllerActionPlayer::ControllerActionPlayer(Player& player) :
	ControllerActionBase(player),
	player_(player)
{
	input_ = nullptr;
	stepJump_ = 0.0f;
	isJump_ = false;
	isEndLanding_ = false;
}

ControllerActionPlayer::~ControllerActionPlayer()
{
}

void ControllerActionPlayer::Load()
{
	//入力管理クラスの生成
	input_ = std::make_unique<InputPlayer>();
}

void ControllerActionPlayer::Init()
{
	//各種変数の初期化
	stepJump_ = 0.0f;
	isJump_ = false;
	isEndLanding_ = false;
}

void ControllerActionPlayer::Update()
{
	//移動操作処理
	ProcessMove();

	//ジャンプ操作処理
	ProcessJump();
}

void ControllerActionPlayer::ProcessMove()
{
	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();

	// 回転したい角度
	double rotDeg = -1;

	// 方向ベクトル
	VECTOR dir = Utility3D::VECTOR_ZERO;

	// 移送速度
	float speed = 0.0f;

	// 右移動
	if (input_->CheckKey(InputPlayer::CONFIG::RIGHT))
	{
		rotDeg = ROT_DEG_RIGHT;
		dir = cameraRot.GetRight();
	}

	// 左移動
	if (input_->CheckKey(InputPlayer::CONFIG::LEFT))
	{
		rotDeg = ROT_DEG_LEFT;
		dir = cameraRot.GetLeft();
	}

	// 前移動
	if (input_->CheckKey(InputPlayer::CONFIG::FORWARD))
	{
		rotDeg = ROT_DEG_FRONT;
		dir = cameraRot.GetForward();
	}

	// 後移動
	if (input_->CheckKey(InputPlayer::CONFIG::BACK))
	{
		rotDeg = ROT_DEG_BACK;
		dir = cameraRot.GetBack();
	}

	// プレイヤーの着地判定
	isEndLanding_ = IsEndLanding();

	// アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// ジャンプ中じゃないかつ操作入力がされたとき
	if (!Utility3D::EqualsVZero(dir) && (isJump_ || isEndLanding_))
	{
		// ダッシュ
		bool isDash = input_->CheckKey(InputPlayer::CONFIG::DASH);

		// 歩く移動速度取得
		speed = player_.GetSpeedMove();

		// ダッシュ中なら速度を変更
		if (isDash)
		{
			speed = player_.GetSpeedRun();
		}

		if (!isJump_ && isEndLanding_)
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
		if (!isJump_ && isEndLanding_)
		{
			animation.Play(Player::ANIM_IDLE);
		}
	}

	// 移動ベクトルの設定
	player_.SetMoveDir(dir);

	// 移動量の設定
	player_.SetMoveSpeed(speed);

	// 回転角度の設定
	player_.SetRotDeg(rotDeg);
}

void ControllerActionPlayer::ProcessJump()
{
	//ジャンプ受付時間
	const float JUMP_ACCEPT_TIME = player_.GetJumpAcceptTime();

	//ジャンプキーを入力したか
	bool isInputJump = input_->CheckKey(InputPlayer::CONFIG::JUMP);

	//アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// ジャンプ
	if (isInputJump && (isJump_ || isEndLanding_))
	{

		if (!isJump_)
		{
			// 制御無しジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP);
			// ループしないジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false);
			// 切り取りアニメーション
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);
			// 無理やりアニメーション
			animation.Play(Player::ANIM_JUMP, true, 13.0f, 25.0f);
			animation.SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		isJump_ = true;

		// ジャンプの入力受付時間をヘラス
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < JUMP_ACCEPT_TIME)
		{
			player_.SetJumpPow(VScale(Utility3D::DIR_U, player_.GetJumpAmount()));
		}

	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isInputJump)
	{
		stepJump_ = JUMP_ACCEPT_TIME;
	}
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