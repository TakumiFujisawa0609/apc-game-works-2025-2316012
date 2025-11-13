#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/ColliderFactory.h"
#include "../../Controller/Draw/ControllerDrawStage.h"
#include "StageObjectBase.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	ActorBase(mapParam),
	STAGE_KEY(key),
	ROOM_TAG(mapParam["tag"])
{
	collider_ = collFtr_.Create(*this, colliderParam);
	isActive_ = true;
	isTrans_ = false;
}

StageObjectBase::~StageObjectBase()
{
}

void StageObjectBase::Load()
{
	// ÉÇÉfÉãÇÃê›íË
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// ï`âÊÇÃê›íË
	draw_ = std::make_unique<ControllerDrawStage>(transform_.modelId);
	draw_->Load();

	// äÓíÍÉNÉâÉXÇÃì«Ç›çûÇ›
	ActorBase::Load();
}

void StageObjectBase::DrawMain()
{
	// ï`âÊ
	draw_->Draw();
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