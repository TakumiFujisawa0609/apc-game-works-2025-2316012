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
#include "../../Controller/OnHit/ControllerOnHitGravity.h"
#include "Player.h"

const std::string Player::ANIM_JUMP = "jump";	//ジャンプ
const std::string Player::ANIM_DIE = "die";		//死ぬ
const std::string Player::ANIM_SLEEP = "sleep";	//眠る

Player::Player(const Json& param) :
	CharacterBase(param),
	JUMP_AMOUNT(param["jumpAmount"]),
	JUMP_ACCEPT_TIME(param["jumpAcceptTime"]),
	ANIM_JUMP_SPEED(param["animationJumpSpeed"]),
	REPORT_INPUT_TIME(param["reportInputTime"]),
	MADNESS_UPDATE_STEP_ADD(param["madnessUpdateStepAdd"]),
	madnessUpdateStep_(param["madnessUpdateStepDefault"])
{	
	stepJump_ = 0.0f;
	madnessValue_ = 0.0f;
	reportPer_ = 0;
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

	// 衝突後の処理クラス
	onHitMap_[CollisionTags::TAG::PLAYER] = std::make_unique<ControllerOnHitPlayer>(*this);
	onHitMap_[CollisionTags::TAG::REPORT] = std::make_unique<ControllerOnHitReport>(*this);
	onHitMap_[CollisionTags::TAG::CHARACTER_GRAVITY_LINE] = std::make_unique<ControllerOnHitGravity>(*this);

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
	std::array frames = { 0,1,2,3,4,5 };
	for (auto cnt : frames)
	{
		MV1DrawFrame(transform_.modelId, cnt);
	}
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

	onHitMap_[CollisionTags::TAG::PLAYER]->Test();
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

	action_->DebugDraw();
}

void Player::SetMadnessValue(const int madnessValue)
{
	// 引数の値を代入
	madnessValue_ = madnessValue;

	// 最大値を超える場合
	if (madnessValue_ > MADNESS_MAX)
	{
		// 最大値を代入
		madnessValue_ = MADNESS_MAX;
	}
}

void Player::AddMadnessValue(const int madnessValue)
{
	// 引数の値を加算
	madnessValue_ += madnessValue;

	// 最大値を超える場合
	if (madnessValue_ > MADNESS_MAX)
	{
		// 最大値を代入
		madnessValue_ = MADNESS_MAX;
	}
}
