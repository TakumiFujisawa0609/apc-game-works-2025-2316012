#include <DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../Controller/ControllerAnimation.h"
#include "../Controller/Action/ControllerActionBase.h"
#include "../Controller/ControllerMove.h"
#include "../Controller/ControllerRotate.h"
#include "../Controller/ControllerGravity.h"
#include "../Controller/OnHit/ControllerOnHitBase.h"
#include "../Collider/ColliderCapsule.h"
#include "../Collider/ColliderLine.h"
#include "CharacterBase.h"

const std::string CharacterBase::ANIM_IDLE = "idle";	// 待機
const std::string CharacterBase::ANIM_WALK = "walk";	// 歩く
const std::string CharacterBase::ANIM_RUN = "run";		// 走る

CharacterBase::CharacterBase(const Json& param) :
	SPEED_MOVE(param["moveSpeed"]),
	SPEED_RUN(param["dashSpeed"]),
	GRAVITY(param["gravity"]),
	TIME_ROT(param["timeRot"]),
	ANIM_DEFAULT_SPEED(param["animationDefaultSpeed"]),
	INITIAL_POS({ param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] }),
	INITIAL_QUA_ROT({ param["initQuaRot"]["x"],param["initQuaRot"]["y"],param["initQuaRot"]["z"] })
{
	rotDeg_ = 0.0f;	
	moveSpeed_ = 0.0f;
	moveDir_ = Utility3D::VECTOR_ZERO;
	jumpPow_ = Utility3D::VECTOR_ZERO;
	animation_ = nullptr;
	action_ = nullptr;
	move_ = nullptr;
	rotate_ = nullptr;
	gravity_ = nullptr;
	onHit_ = nullptr;
}

void CharacterBase::Load()
{
	// アニメーションの制御クラスの生成
	animation_ = std::make_unique<ControllerAnimation>(transform_.modelId);

	// 移動制御クラス
	move_ = std::make_unique<ControllerMove>(*this);

	// 回転制御クラス
	rotate_ = std::make_unique<ControllerRotate>(*this);

	// 重力制御クラス
	gravity_ = std::make_unique<ControllerGravity>(*this);

	// コライダー生成
	auto colliderCapsule = std::make_shared<ColliderCapsule>(*this, COLLISION_TAG::PLAYER);
	colliderCapsule->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	colliderCapsule->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	colliderCapsule->SetRadius(20.0f);
	collider_ = colliderCapsule;
	
	// 衝突判定管理クラスへの追加
	MakeCollider(collider_);

	// アニメーション初期化
	InitAnimation();
}

void CharacterBase::Init()
{	
	// トランスフォームの初期設定
	InitTransform();

	// 移動制御クラスの初期化
	move_->Init();

	// 回転制御クラスの初期化
	rotate_->Init();
}

void CharacterBase::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	onHit_->OnHit(opponentCollider);
}

void CharacterBase::UpdateApply()
{
	// トランスフォームの更新
	transform_.Update();

	// アニメーション制御クラスの更新
	animation_->Update();
}

void CharacterBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}

void CharacterBase::InitTransform()
{
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ INITIAL_QUA_ROT.x, UtilityCommon::Deg2RadF(INITIAL_QUA_ROT.y), INITIAL_QUA_ROT.z });
	transform_.pos = INITIAL_POS;
	transform_.Update();
}

void CharacterBase::DebugDraw()
{
	collider_->DebugDraw();
}

const float CharacterBase::GetCapsuleRadius() const 
{
	const auto& colliderModel = std::dynamic_pointer_cast<ColliderCapsule>(collider_);
	return colliderModel->GetRadius();
}

const VECTOR CharacterBase::GetCapsuleTopPos() const
{
	const auto& colliderModel = std::dynamic_pointer_cast<ColliderCapsule>(collider_);
	return colliderModel->GetPosTop();
}

const VECTOR CharacterBase::GetCapsuleDownPos() const
{
	const auto& colliderModel = std::dynamic_pointer_cast<ColliderCapsule>(collider_);
	return colliderModel->GetPosDown();
}