#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/CollisionTags.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Controller/Draw/ControllerDrawStage.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "StageMain.h"

StageMain::StageMain(const std::string& key, const Json& mapParam, const Json& colliderParam):
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
}

StageMain::~StageMain()
{
}

void StageMain::SetAnomaly()
{
	// モデルのコライダーを生成
	anomalyCollider_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::ANOMALY);

	// コライダーの追加
	collMng_.Add(anomalyCollider_);

	// テクスチャを追加
	draw_->SetTexture(resMng_.GetHandle("bloodyHands"));
}

void StageMain::Refresh()
{
	StageGimmickObjectBase::Refresh();

	// テクスチャを解除
	draw_->SetTexture(-1);
}

bool StageMain::CheckCameraViewClip()
{
	// Boxへ変換
	const auto& colliderBox = std::dynamic_pointer_cast<ColliderBox>(collider_);

	// カメラ外か判定
	if (CheckCameraViewClip_Box(colliderBox->GetVecMax(), colliderBox->GetVecMin()))
	{
		// 視界に入ってない
		return false;
	}
	// 視界に入っている
	return true;
}
