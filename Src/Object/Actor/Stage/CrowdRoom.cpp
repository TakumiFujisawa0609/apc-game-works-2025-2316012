#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Collider/ColliderBase.h"
#include "CrowdRoom.h"

CrowdRoom::CrowdRoom(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageMain(key, mapParam, colliderParam)

{
}

CrowdRoom::~CrowdRoom()
{
}

void CrowdRoom::Draw()
{
	//collider_->DebugDraw();
}

void CrowdRoom::SetAnomaly()
{
	// コライダーの追加
	collMng_.Add(collider_);

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);
}

void CrowdRoom::Refresh()
{
	// コライダー削除
	collider_->SetDelete();

	// ビルボードオブジェクトの削除
	StageManager::GetInstance().DeleteBillboardObjects(StageManager::BILLBOARD_OBJ_TYPE::HUMAN);

	// 自身の削除
	isDelete_ = true;
}