#include "../../Manager/Resource/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "Player.h"

Player::Player()
{
	animType_ = -1;
	state_ = STATE::NONE;

	//状態更新関数の登録
	RegisterStateUpdateFunc(STATE::NONE, std::bind(&Player::UpdateNone, this));
	RegisterStateUpdateFunc(STATE::ALIVE, std::bind(&Player::UpdateAlive, this));
	RegisterStateUpdateFunc(STATE::DEAD, std::bind(&Player::UpdateDead, this));
}

void Player::Load()
{
	//モデルの設定
	transform_.SetModel(resMng_.GetHandle("player"));

	//アニメーションの設定
	animCtrl_ = std::make_unique<AnimationController>(transform_.modelId);
	
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

void Player::InitAnimation()
{
	//アニメーションの登録
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::IDLE), resMng_.GetHandle("playerAnimationIdle"), animDefaultSpeed_);
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::WALK), resMng_.GetHandle("playerAnimationWalking"), animDefaultSpeed_);
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::RUN), resMng_.GetHandle("playerAnimationFastRun"), animDefaultSpeed_);
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::DEAD), resMng_.GetHandle("playerAnimationDie"), animDefaultSpeed_);
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::SLEEP), resMng_.GetHandle("playerAnimationSleepingIdle"), animDefaultSpeed_);
	animCtrl_->Add(static_cast<int>(ANIM_TYPE::JUMP), resMng_.GetHandle("playerAnimationJump"), animDefaultSpeed_);

	//初期アニメーション設定
	animCtrl_->Play(static_cast<int>(ANIM_TYPE::IDLE));
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
