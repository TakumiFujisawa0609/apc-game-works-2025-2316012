#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Controller/Draw/ControllerDrawStage.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "GrassRoom.h"

GrassRoom::GrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageMain(key, mapParam, colliderParam)
{
}

GrassRoom::~GrassRoom()
{
}

void GrassRoom::Load()
{
}

void GrassRoom::Init()
{
}

void GrassRoom::SetAnomaly()
{
	// テクスチャを追加
	draw_->SetTexture(resMng_.GetHandle("bloodyHands"));

	// 報告用にコライダーを設定
	colliderModel_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::DECO_GIMMICK);
	collMng_.Add(colliderModel_);

	// 活動状態の変更
	isActive_ = true;
}

void GrassRoom::Refresh()
{
}
