#include "../../Object/Collider/ColliderBase.h"
#include "../../Object/Collider/ColliderCapsule.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Utility/Utility3D.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
	// 配列サイズ
	const int size = colliders_.size();

	// 衝突判定を行うコライダー
	std::weak_ptr<ColliderBase> collA, collB;

	for (int i = 0; i < size - 1; i++)
	{
		//コライダーが削除予定の場合
		if (colliders_[i].lock()->IsDelete())
		{
			//飛ばす
			continue;
		}

		// コライダーAの種類を設定
		if (colliders_[i].lock()->GetType() == ColliderBase::TYPE::NONE) { continue; }
		else if (colliders_[i].lock()->GetType() == ColliderBase::TYPE::MODEL) { collA = std::dynamic_pointer_cast<ColliderModel>(colliders_[i].lock()); }
		else if (colliders_[i].lock()->GetType() == ColliderBase::TYPE::CAPSULE) { collA = std::dynamic_pointer_cast<ColliderCapsule>(colliders_[i].lock()); }
		else if (colliders_[i].lock()->GetType() == ColliderBase::TYPE::SPHERE) { continue; }

		for (int j = i + 1; j < size; j++)
		{
			// コライダーが削除予定の場合
			if (colliders_[j].lock()->IsDelete())
			{
				// 飛ばす
				continue;
			}


			//if()

			//// 当たり判定
			//if (IsHitCheckModeToCapsule(colliders_[i].lock(), colliders_[j].lock()))
			//{
			//	// それぞれの当たった処理
			//	colliders_[i].lock()->OnHit(colliders_[j]);
			//	colliders_[j].lock()->OnHit(colliders_[i]);

			//	// 当たった後の処理
			//	if (!colliders_[i]->IsDead())
			//	{
			//		colliders_[i]->GetGeometry().HitAfter();
			//	}
			//	if (!colliders_[j]->IsDead())
			//	{
			//		colliders_[j]->GetGeometry().HitAfter();
			//	}
			//}
		}
	}
}

void CollisionManager::Add(const std::shared_ptr<ColliderBase> collider)
{
	// コライダーの追加
	colliders_.push_back(collider);
}

void CollisionManager::Clear()
{
	// 中身の削除
	colliders_.clear();
}

void CollisionManager::Sweep()
{
	// 終了したコライダを並び変える
	auto it = std::remove_if(colliders_.begin(), colliders_.end(),
		[](const std::shared_ptr<ColliderBase> collider)
		{
			return collider->IsDelete();
		});

	// 終了したコライダを削除する
	colliders_.erase(it, colliders_.end());
}

bool CollisionManager::IsHitCheckModeToCapsule(std::weak_ptr<ColliderCapsule> collCapsule, std::weak_ptr<ColliderModel> collModel)
{ 
	//衝突判定
	auto it = MV1CollCheck_Capsule(
		collModel.lock()->GetModel(),
		-1,
		collCapsule.lock()->GetPosTop(),
		collCapsule.lock()->GetPosDown(),
		collCapsule.lock()->GetRadius()
	);
	return it.HitNum >= 1;
}

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}