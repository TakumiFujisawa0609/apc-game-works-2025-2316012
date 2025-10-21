#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
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
#include "../../Controller/OnHit/ControllerOnHitReport.h"
#include "Player.h"

const std::string Player::ANIM_JUMP = "jump";	//ジャンプ
const std::string Player::ANIM_DIE = "die";		//死ぬ
const std::string Player::ANIM_SLEEP = "sleep";	//眠る

Player::Player(const Json& param) :
	CharacterBase(param),
	JUMP_AMOUNT(param["jumpAmount"]),
	JUMP_ACCEPT_TIME(param["jumpAcceptTime"]),
	ANIM_JUMP_SPEED(param["animationJumpSpeed"]),
	REPORT_INPUT_TIME(param["reportInputTime"])
{	
	stepJump_ = 0.0f;
	reportPer_ = 0;
	isJump_ = false;
	onHitReport_ = nullptr;
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

	// 衝突後の処理クラス
	onHit_ = std::make_unique<ControllerOnHitPlayer>(*this);
	onHitReport_ = std::make_unique<ControllerOnHitReport>(*this);

	// 基底クラスの読み込み処理
	CharacterBase::Load();
}

void Player::Init()
{
	// 基底クラスの初期化
	CharacterBase::Init();

	// アクション制御クラスの初期化
	action_->Init();

	// 衝突後処理の初期化
	onHitReport_->Init();

	// 初期状態
	state_ = STATE::ALIVE;
}

void Player::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 衝突相手がプレイヤーの場合
	if (opponentCollider.lock()->GetPartnerTag() == CollisionTags::TAG::PLAYER)
	{
		onHit_->OnHit(opponentCollider);
		return;
	}
	// 衝突相手がレポート用のラインの場合
	else if (opponentCollider.lock()->GetPartnerTag() == CollisionTags::TAG::REPORT)
	{
		onHitReport_->OnHit(opponentCollider);
		return;
	}
	// それ以外の場合
	else
	{
		return;
	}
}

void Player::UpdateBody()
{
	stateUpdateFuncMap_[state_]();
}

void Player::DrawMain()
{
	if (state_ == STATE::DEAD)
	{
		return;
	}

	CharacterBase::DrawMain();
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
	transform_.scl = INITIAL_SCL;
	transform_.pos = INITIAL_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.localPos = { 0.0f,00.0f,0.0f };
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

	VECTOR pos = transform_.pos;
	Quaternion quaRot = transform_.quaRot;
	DrawFormatString(0, 0, UtilityCommon::RED, L"プレイヤー位置：%2f,%2f,%2f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 20, UtilityCommon::RED, L"プレイヤー角度：%2f,%2f,%2f,%2f", quaRot.x, quaRot.y, quaRot.z, quaRot.w);
	DrawFormatString(0, 40, UtilityCommon::RED, L"プレイヤー目標角度：%2f,%2f,%2f,%2f", goalQuaRot_.x, goalQuaRot_.y, goalQuaRot_.z, goalQuaRot_.w);
	DrawFormatString(0, 140, UtilityCommon::RED, L"回転角度ステップ：%2f", rotStep_);

}