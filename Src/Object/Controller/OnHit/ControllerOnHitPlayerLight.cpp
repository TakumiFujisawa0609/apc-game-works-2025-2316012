#include "../../Actor/Character/Player.h"
#include "../../Collider/ColliderModel.h"
#include "../ControllerLight.h"
#include "ControllerOnHitPlayerLight.h"

ControllerOnHitPlayerLight::ControllerOnHitPlayerLight(Player& owner):
	owner_(owner),
	light_(owner.GetControllerLight())
{
	// 衝突物別関数の登録
	RegisterOnHit(CollisionTags::TAG::STAGE_MAIN, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitMainStage(opponentCollider); });
}

ControllerOnHitPlayerLight::~ControllerOnHitPlayerLight()
{
}

void ControllerOnHitPlayerLight::OnHitMainStage(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// モデル用コライダーへ変換
	auto collModel = std::dynamic_pointer_cast<ColliderModel>(opponentCollider.lock());

	// 衝突情報の取得
	const auto& info = collModel->GetCollResultPoly();

	// 衝突位置の格納
	light_.SetLightPos(info.HitPosition);
}