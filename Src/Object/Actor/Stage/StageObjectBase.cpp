#include "StageObjectBase.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/ColliderFactory.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	ActorBase(mapParam),
	STAGE_KEY(key)
{
	collider_ = collFtr_.Create(*this, colliderParam);
}

StageObjectBase::~StageObjectBase()
{
}

void StageObjectBase::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// 基底クラスの読み込み
	ActorBase::Load();
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
	transform_.scl = INITIAL_SCL;
	transform_.pos = VScale(INITIAL_POS, UtilityCommon::METER_TO_UNIT_SCALE);
	transform_.quaRot = Quaternion::Euler({ UtilityCommon::Deg2RadF(INITIAL_ROT.x),UtilityCommon::Deg2RadF(INITIAL_ROT.y), UtilityCommon::Deg2RadF(INITIAL_ROT.z )});
	transform_.quaRotLocal = Quaternion::Euler({ UtilityCommon::Deg2RadF(0.0f),UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void StageObjectBase::DebugDraw()
{
	if (collider_ == nullptr)
	{
		return;
	}

	collider_->DebugDraw();
}
