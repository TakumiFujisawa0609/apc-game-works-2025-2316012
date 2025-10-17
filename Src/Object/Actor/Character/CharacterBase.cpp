#include <DxLib.h>
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Controller/Action/ControllerActionBase.h"
#include "../../Controller/ControllerMove.h"
#include "../../Controller/ControllerRotate.h"
#include "../../Controller/ControllerGravity.h"
#include "../../Controller/OnHit/ControllerOnHitBase.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../Collider/ColliderFactory.h"
#include "CharacterBase.h"

const std::string CharacterBase::ANIM_IDLE = "idle";	// 待機
const std::string CharacterBase::ANIM_WALK = "walk";	// 歩く
const std::string CharacterBase::ANIM_RUN = "run";		// 走る

CharacterBase::CharacterBase(const Json& param) :
	ActorBase(param),
	SPEED_MOVE(param["moveSpeed"]),
	SPEED_RUN(param["dashSpeed"]),
	GRAVITY(param["gravity"]),
	TIME_ROT(param["timeRot"]),
	ANIM_DEFAULT_SPEED(param["animationDefaultSpeed"])
{
	rotStep_ = 0.0f;	
	moveSpeed_ = 0.0f;
	prePos_ = Utility3D::VECTOR_ZERO;
	moveDir_ = Utility3D::VECTOR_ZERO;
	jumpPow_ = Utility3D::VECTOR_ZERO;
	animation_ = nullptr;
	action_ = nullptr;
	move_ = nullptr;
	rotate_ = nullptr;
	gravity_ = nullptr;
	onHit_ = nullptr;
	goalQuaRot_ = Quaternion();
	collider_ = collFtr_.Create(*this, param);
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

	// アニメーション初期化
	InitAnimation();

	// 基底クラスの読み込み
	ActorBase::Load();
}

void CharacterBase::Init()
{	
	// トランスフォームの初期設定
	InitTransform();

	// アクションクラスの初期化
	action_->Init();

	// 移動制御クラスの初期化
	move_->Init();

	// 回転制御クラスの初期化
	rotate_->Init();

	// 衝突後クラスの初期化
	onHit_->Init();
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