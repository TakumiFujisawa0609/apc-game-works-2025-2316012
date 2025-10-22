#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Controller/Action/ControllerActionBase.h"
#include "../../Controller/ControllerMove.h"
#include "../../Controller/ControllerRotate.h"
#include "../../Controller/ControllerGravity.h"
#include "../../Controller/OnHit/ControllerOnHitBase.h"
#include "../../Controller/OnHit/ControllerOnHitGhost.h"
#include "../../Collider/ColliderCapsule.h"
#include "Ghost.h"

Ghost::Ghost(const Json& param) :
	CharacterBase(param)
{
}

Ghost::~Ghost()
{
}

void Ghost::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle("ghost"));

	// ゴーストの衝突後の処理
	onHit_ = std::make_unique<ControllerOnHitGhost>(*this);

	CharacterBase::Load();
}

void Ghost::Init()
{
	// アニメーション初期化
	InitAnimation();
}

void Ghost::Create(const Json& param)
{
	// 読み込み処理
	Load();

	// 初期化処理
	Init();

	// 位置や角度の設定
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.pos = { param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] };
	transform_.pos = VScale(transform_.pos, UtilityCommon::METER_TO_UNIT_SCALE);
	VECTOR rot = { param["initRot"]["x"],param["initRot"]["y"],param["initRot"]["z"] };
	transform_.quaRot = Quaternion::Euler({ UtilityCommon::Deg2RadF(rot.x),UtilityCommon::Deg2RadF(rot.y), UtilityCommon::Deg2RadF(rot.z) });
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void Ghost::DrawMain()
{
	CharacterBase::DrawMain();
}

void Ghost::InitAnimation()
{
	animation_->Add(ANIM_IDLE, resMng_.GetHandle("ghostAnimationIdle"), ANIM_DEFAULT_SPEED);
	animation_->Play(ANIM_IDLE);
}