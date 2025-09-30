#include "StageObjectBase.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../Collider/ColliderModel.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& param) :
	ActorBase(param),
	STAGE_KEY(key)
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
	MakeCollider(*this);
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