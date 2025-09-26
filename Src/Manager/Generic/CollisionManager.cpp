#include "../../Object/Collider/ColliderBase.h"
#include "../../Object/Collider/ColliderCapsule.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Object/Collider/ColliderLine.h"
#include "../../Utility/Utility3D.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
	// 配列サイズ
	const int size = colliders_.size();

	for (int i = 0; i < size - 1; i++)
	{
		// コライダーが削除予定の場合
		if (colliders_[i]->IsDelete())
		{
			// 次へ
			continue;
		}

		for (int j = i + 1; j < size; j++)
		{
			// コライダーが削除予定の場合
			if (colliders_[j]->IsDelete())
			{
				// 次へ
				continue;
			}

			// 各コライダーからタグを取得
			const auto& tag1 = colliders_[i]->GetTag();
			const auto& tag2 = colliders_[j]->GetTag();

			// 衝突判定が不要な組み合わせの場合
			if (!collTagMatrix_[static_cast<int>(tag1)][static_cast<int>(tag2)])
			{
				// 次へ
				continue;
			}

			// 各コライダーから種類を取得
			const auto& type1 = colliders_[i]->GetType();
			const auto& type2 = colliders_[j]->GetType();

			// 衝突判定関数を取得
			auto& collisionFunction = collFuncMatrix_[static_cast<int>(type1)][static_cast<int>(type2)];

			// 関数が登録されていない場合
			if (!collisionFunction)
			{
				// 次へ
				continue;
			}
				// 衝突判定を実行
			if (collisionFunction(colliders_[i], colliders_[j]))
			{
				// それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j]);
				colliders_[j]->OnHit(colliders_[i]);
			}
		}
	}
}

void CollisionManager::Add(std::shared_ptr<ColliderBase> collider)
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
		[](const std::weak_ptr<ColliderBase> collider)
		{
			return collider.lock()->IsDelete();
		});

	// 終了したコライダを削除する
	colliders_.erase(it, colliders_.end());
}

void CollisionManager::InitTagMatrix()
{
	// サイズの定義
	collTagMatrix_.resize(static_cast<int>(COLLISION_TAG::MAX), std::vector<bool>(static_cast<int>(COLLISION_TAG::MAX), false));

	// 衝突判定を行う組み合わせを設定
	collTagMatrix_[static_cast<int>(COLLISION_TAG::PLAYER)][static_cast<int>(COLLISION_TAG::STAGE)] = true;		// プレイヤーとステージ
}

void CollisionManager::InitColliderMatrix()
{
	// サイズの定義
	const int max = static_cast<int>(ColliderBase::TYPE::MAX);	// 要素の最大数
	collFuncMatrix_.resize(max, std::vector<std::function<bool(std::weak_ptr<ColliderBase>, std::weak_ptr<ColliderBase>)>>(max));

	// 特定の組み合わせの関数を代入
	collFuncMatrix_[static_cast<int>(ColliderBase::TYPE::MODEL)][static_cast<int>(ColliderBase::TYPE::CAPSULE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToCapsule(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderBase::TYPE::CAPSULE)][static_cast<int>(ColliderBase::TYPE::MODEL)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToCapsule(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderBase::TYPE::MODEL)][static_cast<int>(ColliderBase::TYPE::LINE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToLine(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderBase::TYPE::LINE)][static_cast<int>(ColliderBase::TYPE::MODEL)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToLine(collA, collB);
		};
}

bool CollisionManager::IsHitCheckModeToCapsule(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderModel> collModel;
	std::weak_ptr<ColliderCapsule> collCapsule;

	// モデルコライダーの用意
	if (collA.lock()->GetType() == ColliderBase::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderBase::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collB.lock()); }

	// カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderBase::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderBase::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collB.lock()); }

	// 衝突判定
	auto it = MV1CollCheck_Capsule(
		collModel.lock()->GetModel(),
		-1,
		collCapsule.lock()->GetPosTop(),
		collCapsule.lock()->GetPosDown(),
		collCapsule.lock()->GetRadius()
	);

	// 衝突後の情報を格納
	collModel.lock()->SetCollResultPolyDim(it);

	// 衝突しているか返す
	return it.HitNum >= 1;
}

bool CollisionManager::IsHitCheckModeToLine(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderModel> collModel;
	std::weak_ptr<ColliderLine> collLine;

	//モデルコライダーの用意
	if (collA.lock()->GetType() == ColliderBase::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderBase::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collB.lock()); }

	//カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderBase::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderBase::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collB.lock()); }

	// 衝突判定
	auto it = MV1CollCheck_Line(
		collModel.lock()->GetModel(),
		-1,
		collLine.lock()->GetLocalPosPointHead(),
		collLine.lock()->GetLocalPosPointEnd()
	);

	// 衝突後の情報を格納
	collModel.lock()->SetCollResultPoly(it);

	// 衝突しているか返す
	return it.HitFlag;
}

CollisionManager::CollisionManager()
{
	// 衝突判定の組み合わせを初期化
	InitTagMatrix();

	// 形状別判定組み合わせを初期化
	InitColliderMatrix();
}

CollisionManager::~CollisionManager()
{
}