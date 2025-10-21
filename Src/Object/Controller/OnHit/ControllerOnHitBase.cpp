#include "../../../Manager/Generic/GameStateManager.h"
#include "../../Collider/ColliderBase.h"
#include "ControllerOnHitBase.h"

ControllerOnHitBase::ControllerOnHitBase() :
    stateMng_(GameStateManager::GetInstance())
{
}

ControllerOnHitBase::~ControllerOnHitBase()
{
}

void ControllerOnHitBase::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{       
    auto collider = opponentCollider.lock();

    // 中身の確認
    if (!collider) 
    {
        // 無効なポインタの場合は何もしない
        return; 
    }

    // 登録されていない場合
    if (onHitMap_.count(collider->GetTag()) == 0)
    {
        // 処理を行わずに終了
        return;
    }

    // 衝突物別の処理へ
	onHitMap_[collider->GetTag()](opponentCollider);
}

void ControllerOnHitBase::Init()
{
}

void ControllerOnHitBase::RegisterOnHit(const CollisionTags::TAG tag, std::function<void(const std::weak_ptr<ColliderBase>&)> onHit)
{
	onHitMap_[tag] = onHit;
}