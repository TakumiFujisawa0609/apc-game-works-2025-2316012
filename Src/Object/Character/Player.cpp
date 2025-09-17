#include "../../Manager/Resource/ResourceManager.h"
#include "../Common/Controller/ControllerAnimation.h"
#include "../System/InputPlayer.h"
#include "Parameter/ParameterPlayer.h"
#include "Player.h"

Player::Player(ParameterPlayer& parameter) :
	parameter_(parameter)
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
	//パラメーターからアニメーション速度を取得
	float animationDefaultSpeed = parameter_.animationDefaultSpeed_;
	float animationJumpSpeed = parameter_.animationJumpSpeed_;

	//アニメーションの登録
	animation_->Add(static_cast<int>(ANIM_TYPE::IDLE), resMng_.GetHandle("playerAnimationIdle"), animationDefaultSpeed);
	animation_->Add(static_cast<int>(ANIM_TYPE::WALK), resMng_.GetHandle("playerAnimationWalking"), animationDefaultSpeed);
	animation_->Add(static_cast<int>(ANIM_TYPE::RUN), resMng_.GetHandle("playerAnimationFastRun"), animationDefaultSpeed);
	animation_->Add(static_cast<int>(ANIM_TYPE::DEAD), resMng_.GetHandle("playerAnimationDie"), animationDefaultSpeed);
	animation_->Add(static_cast<int>(ANIM_TYPE::SLEEP), resMng_.GetHandle("playerAnimationSleepingIdle"), animationDefaultSpeed);
	animation_->Add(static_cast<int>(ANIM_TYPE::JUMP), resMng_.GetHandle("playerAnimationJump"), animationJumpSpeed);

	//初期アニメーション設定
	animation_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::RegisterStateUpdateFunc(const STATE state, std::function<void()> update)
{
	stateUpdateFuncMap_[state] = update;
}

void Player::UpdateNone()
{
}

void Player::UpdateAlive()
{
}

void Player::UpdateDead()
{
}