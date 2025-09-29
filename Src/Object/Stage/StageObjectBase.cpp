#include "StageObjectBase.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../Collider/ColliderModel.h"
#include "../Common/Quaternion.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& param) :
	STAGE_KEY(key),
	INITIAL_POS({ param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] }),
	INITIAL_ROT({ param["initRot"]["x"],param["initRot"]["y"],param["initRot"]["z"] }),
	INITIAL_SCL({ param["initScl"]["x"],param["initScl"]["y"],param["initScl"]["z"] })
{
}

StageObjectBase::~StageObjectBase()
{
}

void StageObjectBase::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle("mainStage"));

	// コライダーの生成
	collider_ = std::make_shared<ColliderModel>(*this, COLLISION_TAG::STAGE);
	MakeCollider(collider_);
}

void StageObjectBase::Init()
{
	transform_.quaRot = Quaternion();
	transform_.scl = INITIAL_SCL;
	transform_.quaRotLocal = Quaternion::Euler({ UtilityCommon::Deg2RadF(INITIAL_ROT.x), UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y),UtilityCommon::Deg2RadF(INITIAL_ROT.z) });
	transform_.pos = INITIAL_POS;
	transform_.Update();
}

void StageObjectBase::UpdateApply()
{	
	transform_.Update();
}

void StageObjectBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}