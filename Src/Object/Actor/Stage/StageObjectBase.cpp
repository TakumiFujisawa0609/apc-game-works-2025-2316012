#include "StageObjectBase.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/ColliderFactory.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& param) :
	ActorBase(param),
	STAGE_KEY(key),
	colliderRadius_(param["colliderRadius"]),
	colliderHeadPos_({ param["colliderHeadPos"]["x"],param["colliderHeadPos"]["y"],param["colliderHeadPos"]["z"] }),
	colliderEndPos_({ param["colliderEndPos"]["x"],param["colliderEndPos"]["y"],param["colliderEndPos"]["z"] })
{
}

StageObjectBase::~StageObjectBase()
{
}

void StageObjectBase::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// コライダーの生成
	MakeCollider();
}

void StageObjectBase::Init()
{
	// 基底クラスの初期化
	ActorBase::Init();
}

void StageObjectBase::UpdateApply()
{	
	transform_.Update();
}

void StageObjectBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}

void StageObjectBase::InitTransform()
{
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.rot = INITIAL_ROT;
	transform_.pos = VScale(INITIAL_POS, METER_TO_UNIT_SCALE);
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y),0.0f });
	transform_.Update();
}

void StageObjectBase::MakeCollider()
{
	// 情報の設定
	ColliderFactory::ColliderInfo info;
	info.tag = COLLISION_TAG;
	info.type = COLLIDER_TYPE;
	info.radius = colliderRadius_;
	info.headPos = colliderHeadPos_;
	info.endPos = colliderEndPos_;

	// コライダー生成
	collider_ = collFtr_.Create(*this, info);

	// 管理クラスに格納
	collMng_.Add(collider_);
}

void StageObjectBase::DebugDraw()
{
	collider_->DebugDraw();
}
