#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Controller/ControllerMove.h"
#include "../../Controller/ControllerRotate.h"
#include "../../Controller/ControllerGravity.h"
#include "../../Controller/Action/ControllerActionPlayer.h"
#include "../../Controller/OnHit/ControllerOnHitPlayer.h"
#include "Player.h"

const std::string Player::ANIM_JUMP = "jump";	//ジャンプ
const std::string Player::ANIM_DIE = "die";		//死ぬ
const std::string Player::ANIM_SLEEP = "sleep";	//眠る

Player::Player(const Json& param) :
	CharacterBase(param),
	JUMP_AMOUNT(param["jumpAmount"]),
	JUMP_ACCEPT_TIME(param["jumpAcceptTime"]),
	ANIM_JUMP_SPEED(param["animationJumpSpeed"])
{	
	stepJump_ = 0.0f;
	isJump_ = false;
	state_ = STATE::NONE;

	// 状態更新関数の登録
	RegisterStateUpdateFunc(STATE::NONE, std::bind(&Player::UpdateNone, this));
	RegisterStateUpdateFunc(STATE::ALIVE, std::bind(&Player::UpdateAlive, this));
	RegisterStateUpdateFunc(STATE::DEAD, std::bind(&Player::UpdateDead, this));
}

Player::~Player()
{
}

void Player::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle("player"));

	// アクション制御クラスの生成
	action_ = std::make_unique<ControllerActionPlayer>(*this);
	action_->Load();

	// 衝突後の処理クラス
	onHit_ = std::make_unique<ControllerOnHitPlayer>(*this);

	// 基底クラスの読み込み処理
	CharacterBase::Load();
}

void Player::Init()
{
	// 基底クラスの初期化
	CharacterBase::Init();

	// アクション制御クラスの初期化
	action_->Init();

	// 初期状態
	state_ = STATE::ALIVE;
}

void Player::UpdateMain()
{
	stateUpdateFuncMap_[state_]();
}

void Player::UpdateApply()
{
	// 親クラスの処理を実行
	CharacterBase::UpdateApply();
}

void Player::InitAnimation()
{
	// アニメーションの登録
	animation_->Add(ANIM_IDLE, resMng_.GetHandle("playerAnimationIdle"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_WALK, resMng_.GetHandle("playerAnimationWalking"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_RUN, resMng_.GetHandle("playerAnimationFastRun"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_DIE, resMng_.GetHandle("playerAnimationDie"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_SLEEP, resMng_.GetHandle("playerAnimationSleepingIdle"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_JUMP, resMng_.GetHandle("playerAnimationJump"), ANIM_JUMP_SPEED);

	// 初期アニメーション設定
	animation_->Play(ANIM_IDLE);
}

void Player::InitTransform()
{
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ INITIAL_ROT.x, UtilityCommon::Deg2RadF(INITIAL_ROT.y), INITIAL_ROT.z });
	transform_.pos = INITIAL_POS;
	transform_.localPos = { 0.0f,20.0f,0.0f };
	transform_.Update();
}

void Player::RegisterStateUpdateFunc(const STATE state, std::function<void()> update)
{
	stateUpdateFuncMap_[state] = update;
}

void Player::UpdateAlive()
{
	// アクション
	action_->Update();

	// 重力
	gravity_->Update();	

	// 回転
	rotate_->Update();
	
	// 移動
	move_->Update();
}

void Player::UpdateDead()
{
}

void Player::DebugDraw()
{
	CharacterBase::DebugDraw();
	animation_->DebugDraw();
}