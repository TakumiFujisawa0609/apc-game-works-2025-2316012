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
#include "../../Controller/ControllerPathFinder.h"
#include "../../Controller/Action/ControllerActionEnemy.h"
#include "../../Controller/OnHit/ControllerOnHitEnemy.h"
#include "../../Controller/OnHit/ControllerOnHitEnemyView.h"
#include "Enemy.h"

const std::string Enemy::ANIM_ACTION = "action";		// 攻撃

Enemy::Enemy(const Json& param) :
	CharacterBase(param),
	FIRST_POS_INDEX(param["firstPosIndex"]),
	VIEW_ANGLE(param["viewAngle"]),
	VIEW_RANGE(param["viewRange"])
{
	pathFinder_ = nullptr;
	onHitView_ = nullptr;
	state_ = STATE::NONE;
	movePosList_.clear();
	// 状態更新関数の登録
	RegisterStateUpdateFunc(STATE::NONE, std::bind(&Enemy::UpdateNone, this));
	RegisterStateUpdateFunc(STATE::ALIVE, std::bind(&Enemy::UpdateAlive, this));
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
		VECTOR pos = { it["x"], it["y"], it["z"] };
		movePosList_.emplace_back(pos);
	}

	// 経路探索処理の生成
	pathFinder_ = std::make_unique<ControllerPathFinder>();
	pathFinder_->SetPoints(movePosList_);	
	
	// アクション処理の設定
	action_ = std::make_unique<ControllerActionEnemy>(*this);	
	
	// 判定後処理の生成
	onHit_ = std::make_unique<ControllerOnHitEnemy>(*this);

	// 視野用の衝突後処理の生成
	onHitView_ = std::make_unique<ControllerOnHitEnemyView>(*this);
}

void Enemy::Init()
{
	// 基底クラスの初期化
	CharacterBase::Init();

	// 初期状態
	state_ = STATE::ALIVE;	
	
	// 敵の回転速度
	rotStep_ = 0.1f;
}

void Enemy::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 衝突相手が敵の場合
	if (opponentCollider.lock()->GetPartnerTag() == CollisionTags::TAG::ENEMY)
	{
		onHit_->OnHit(opponentCollider);
		return;
	}
	// 衝突相手が敵の視野用ラインの場合
	else if (opponentCollider.lock()->GetPartnerTag() == CollisionTags::TAG::ENEMY_VIEW)
	{
		onHitView_->OnHit(opponentCollider);
		return;
	}
	// それ以外の場合
	else
	{
		return;
	}
}

ControllerActionEnemy* Enemy::GetActionEnemy()
{
	// アクションクラスの取得
	auto action = dynamic_cast<ControllerActionEnemy*>(action_.get());

	// キャストに失敗した場合
	if (action == nullptr)
	{
		// nullptrを返す
		return nullptr;
	}
	return action;
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

	// 初期アニメーションの再生
	animation_->Play(ANIM_IDLE);
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

void Enemy::UpdateAlive()
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

void Enemy::DebugDraw()
{
	// 基底クラスのデバッグ描画
	CharacterBase::DebugDraw();

	// 自身の動範囲を描画
	DrawSphere3D(transform_.pos, ControllerActionEnemy::ADJACENT_NODE_DIST, 8, UtilityCommon::YELLOW, UtilityCommon::YELLOW, false);

	// 位置情報の表示
	VECTOR pos = transform_.pos;
	DrawFormatString(0, 220, UtilityCommon::WHITE, L"敵の位置 :%2f,%2f, %2f", pos.x, pos.y, pos.z);

	// 視野角の描画
	action_->DebugDraw();
}