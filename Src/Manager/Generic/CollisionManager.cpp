#include "../../Object/Collider/ColliderBase.h"
#include "../../Object/Collider/ColliderCapsule.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
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
	//終了したコライダを並び変える
	auto it = std::remove_if(colliders_.begin(), colliders_.end(),
		[](const std::shared_ptr<ColliderBase> collider)
		{
			return collider->IsDelete();
		});

	//終了したコライダを削除する
	colliders_.erase(it, colliders_.end());
}

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}