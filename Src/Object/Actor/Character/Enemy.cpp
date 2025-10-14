#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/UtilityLoad.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Controller/ControllerMove.h"
#include "../../Controller/ControllerRotate.h"
#include "../../Controller/ControllerGravity.h"
#include "../../Controller/Action/ControllerActionPlayer.h"
#include "../../Controller/OnHit/ControllerOnHitBase.h"
#include "Enemy.h"

const std::string Enemy::ANIM_ACTION = "action";		// 攻撃

Enemy::Enemy(const Json& param) :
	CharacterBase(param)
{
	state_ = STATE::NONE;

	// 状態更新関数の登録
	RegisterStateUpdateFunc(STATE::NONE, std::bind(&Enemy::UpdateNone, this));
	RegisterStateUpdateFunc(STATE::SEARCH, std::bind(&Enemy::UpdateSearch, this));
	RegisterStateUpdateFunc(STATE::CHASE, std::bind(&Enemy::UpdateChase, this));
	RegisterStateUpdateFunc(STATE::ACTION, std::bind(&Enemy::UpdateAction, this));
}

Enemy::~Enemy()
{
}

void Enemy::Load()
{
	// リソースの設定
	transform_.SetModel(resMng_.GetHandle("enemy"));

	// 基底クラスの読み込み
	CharacterBase::Load();

	// 座標リスト
	auto posList = UtilityLoad::GetJsonArrayData("EnemyPosList");

	//　格納
	for (auto& it : posList)
	{
		VECTOR pos = { it["pos"]["x"], it["pos"]["y"], it["pos"]["z"] };
		pos = VScale(pos, 100.0f);
		movePosList_.emplace_back(pos);
	}

	onHit_ = std::make_unique<ControllerOnHitBase>();
}

void Enemy::Init()
{
	// 基底クラスの初期化
	CharacterBase::Init();

	// 初期状態
	state_ = STATE::SEARCH;
}

void Enemy::UpdateBody()
{
	stateUpdateFuncMap_[state_]();
}

void Enemy::InitAnimation()
{
	// アニメーションの登録
	animation_->Add(ANIM_IDLE, resMng_.GetHandle("enemyAnimationIdle"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_WALK, resMng_.GetHandle("enemyAnimationWalking"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_RUN, resMng_.GetHandle("enemyAnimationFastRun"), ANIM_DEFAULT_SPEED);
	animation_->Add(ANIM_ACTION, resMng_.GetHandle("enemyAnimationAction"), ANIM_DEFAULT_SPEED);
	animation_->Play(ANIM_IDLE, true);
}

void Enemy::InitTransform()
{
	transform_.scl = INITIAL_SCL;
	transform_.pos = INITIAL_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.localPos = { 0.0f,0.0f,0.0f };
	transform_.Update();
}

void Enemy::RegisterStateUpdateFunc(const STATE state, std::function<void()> update)
{
	stateUpdateFuncMap_[state] = update;	
}

void Enemy::UpdateSearch()
{
}

void Enemy::UpdateChase()
{
}

void Enemy::UpdateAction()
{
}

void Enemy::DebugDraw()
{
	// 基底クラスのデバッグ描画
	CharacterBase::DebugDraw();
}
