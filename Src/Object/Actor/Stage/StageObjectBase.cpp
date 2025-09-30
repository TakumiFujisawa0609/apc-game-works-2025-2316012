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