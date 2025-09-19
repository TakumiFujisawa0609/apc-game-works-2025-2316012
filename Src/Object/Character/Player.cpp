#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../Common/Controller/ControllerAnimation.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../System/InputPlayer.h"
#include "Player.h"

Player::Player(const Json& param) :
	CharacterBase(param),
	POW_JUMP(param["jumpPower"]),
	JUMP_ACCEPT_TIME(param["jumpAcceptTime"]),
	ANIM_JUMP_SPEED(param["animationJumpSpeed"])
{	
	state_ = STATE::NONE;
	animation_ = nullptr;
	inputPlayer_ = nullptr;

	//状態更新関数の登録
	RegisterStateUpdateFunc(STATE::NONE, std::bind(&Player::UpdateNone, this));
	RegisterStateUpdateFunc(STATE::ALIVE, std::bind(&Player::UpdateAlive, this));
	RegisterStateUpdateFunc(STATE::DEAD, std::bind(&Player::UpdateDead, this));
}

void Player::Load()
{
	//モデルの設定
	transform_.SetModel(resMng_.GetHandle("player"));

	//入力管理クラスの生成
	inputPlayer_ = std::make_unique<InputPlayer>();

	//アニメーションの設定
	animation_ = std::make_unique<ControllerAnimation>(transform_.modelId);
	
	//アニメーションの設定
	InitAnimation();
}

void Player::Init()
{
	//基底クラスの初期化
	CharacterBase::Init();

	//初期状態
	state_ = STATE::ALIVE;
}

void Player::UpdateMain()
{
	stateUpdateFuncMap_[state_]();
}

void Player::UpdateApply()
{
	//親クラスの処理を実行
	CharacterBase::UpdateApply();

	//アニメーションの更新
	animation_->Update();
}

void Player::InitAnimation()
{
	//アニメーションの登録
	animation_->Add(static_cast<int>(ANIM_TYPE::IDLE), resMng_.GetHandle("playerAnimationIdle"), ANIM_DEFAULT_SPEED);
	animation_->Add(static_cast<int>(ANIM_TYPE::WALK), resMng_.GetHandle("playerAnimationWalking"), ANIM_DEFAULT_SPEED);
	animation_->Add(static_cast<int>(ANIM_TYPE::RUN), resMng_.GetHandle("playerAnimationFastRun"), ANIM_DEFAULT_SPEED);
	animation_->Add(static_cast<int>(ANIM_TYPE::DEAD), resMng_.GetHandle("playerAnimationDie"), ANIM_DEFAULT_SPEED);
	animation_->Add(static_cast<int>(ANIM_TYPE::SLEEP), resMng_.GetHandle("playerAnimationSleepingIdle"), ANIM_DEFAULT_SPEED);
	animation_->Add(static_cast<int>(ANIM_TYPE::JUMP), resMng_.GetHandle("playerAnimationJump"), ANIM_JUMP_SPEED);

	//初期アニメーション設定
	animation_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::RegisterStateUpdateFunc(const STATE state, std::function<void()> update)
{
	stateUpdateFuncMap_[state] = update;
}

void Player::UpdateAlive()
{
	//操作処理
	ProcessMove();

	//移動方向に応じた回転
	Rotate();
}

void Player::UpdateDead()
{
}

void Player::ProcessMove()
{
	//移動ベクトル
	movePower_ = Utility3D::VECTOR_ZERO;

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();

	// 回転したい角度
	double rotRad = 0;

	//方向ベクトル
	VECTOR dir = Utility3D::VECTOR_ZERO;

	//右移動
	if (inputPlayer_->CheckKey(InputPlayer::CONFIG::RIGHT))
	{
		rotRad = UtilityCommon::Deg2RadD(90.0);
		dir = cameraRot.GetRight();
	}

	//左移動
	if (inputPlayer_->CheckKey(InputPlayer::CONFIG::LEFT))
	{
		rotRad = UtilityCommon::Deg2RadD(270.0);
		dir = cameraRot.GetLeft();
	}

	//前移動
	if (inputPlayer_->CheckKey(InputPlayer::CONFIG::FORWARD))
	{
		rotRad = UtilityCommon::Deg2RadD(0.0);
		dir = cameraRot.GetForward();
	}

	//後移動
	if (inputPlayer_->CheckKey(InputPlayer::CONFIG::BACK))
	{
		rotRad = UtilityCommon::Deg2RadD(180.0);
		dir = cameraRot.GetBack();
	}
	
	bool isEndLanding = IsEndLanding();

	//ジャンプ中じゃないかつ操作入力がされたとき
	if (!Utility3D::EqualsVZero(dir) && (isJump_ || isEndLanding))
	{	
		//ダッシュ
		bool isDash = inputPlayer_->CheckKey(InputPlayer::CONFIG::DASH);
		
		// 移動処理
		float speed = SPEED_MOVE;

		// ダッシュ中なら速度を変更
		if (isDash)
		{
			speed = SPEED_RUN;
		}

		// 移動ベクトルの設定
		moveDir_ = dir;
		movePower_ = VScale(dir, speed);

		// 回転処理
		SetGoalRotate(rotRad);

		if (!isJump_ && isEndLanding)
		{
			// アニメーション
			if (isDash)
			{
				animation_->Play((int)ANIM_TYPE::RUN);
			}
			else
			{
				animation_->Play((int)ANIM_TYPE::WALK);
			}
		}
	}
	else
	{
		if (!isJump_ && isEndLanding)
		{
			animation_->Play((int)ANIM_TYPE::IDLE);
		}
	}
}

void Player::ProcessJump()
{
}

bool Player::IsEndLanding(void)
{
	// アニメーションがジャンプではない
	if (animation_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return true;
	}

	// アニメーションが終了しているか
	if (animation_->IsEnd())
	{
		return true;
	}

	return false;
}

void Player::DebugDraw()
{
	CharacterBase::DebugDraw();
	animation_->DebugDraw();
}
