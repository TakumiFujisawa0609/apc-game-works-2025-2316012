#include "../../Object/Collider/ColliderBase.h"
#include "../../Object/Collider/ColliderCapsule.h"
#include "../../Object/Collider/ColliderBox.h"
#include "../../Object/Collider/ColliderModel.h"
#include "../../Object/Collider/ColliderLine.h"
#include "../../Object/Collider/ColliderType.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "CollisionTags.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
	// 配列サイズ
	const int size = static_cast<int>(colliders_.size());

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			// 各コライダーからタグを取得
			const auto& tag1 = colliders_[i]->GetTag();
			const auto& tag2 = colliders_[j]->GetTag();

			if (tag1 == CollisionTags::TAG::GHOST && tag2 == CollisionTags::TAG::REPORT ||
				tag1 == CollisionTags::TAG::REPORT && tag2 == CollisionTags::TAG::GHOST)
			{
				// YEAR
				int a = 0;
			}

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
				// お互いに相手のタグを設定
				colliders_[i]->SetPertnerTag(tag2);
				colliders_[j]->SetPertnerTag(tag1);

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

void CollisionManager::DebugDraw()
{
	DrawFormatString(500, 0, UtilityCommon::RED, L"コライダーの数 : %d", colliders_.size());
}

void CollisionManager::InitTagMatrix()
{
	// サイズの定義
	collTagMatrix_.resize(CollisionTags::TAG_COUNT, std::vector<bool>(CollisionTags::TAG_COUNT, false));

	// 衝突判定を行う組み合わせを設定
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::PLAYER)][static_cast<int>(CollisionTags::TAG::MAIN_STAGE)] = true;		// プレイヤーとステージ
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::PLAYER)][static_cast<int>(CollisionTags::TAG::STAGE_GIMMICK)] = true;	// プレイヤーとステージオブジェクト
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::REPORT)][static_cast<int>(CollisionTags::TAG::STAGE_GIMMICK)] = true;	// レポート用ラインとステージオブジェクト
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::STAGE_GIMMICK)][static_cast<int>(CollisionTags::TAG::REPORT)] = true;	// レポート用ラインとステージオブジェクト
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::REPORT)][static_cast<int>(CollisionTags::TAG::GHOST)] = true;			// レポート用ラインとゴースト
	collTagMatrix_[static_cast<int>(CollisionTags::TAG::GHOST)][static_cast<int>(CollisionTags::TAG::REPORT)] = true;			// レポート用ラインとゴースト
}

void CollisionManager::InitColliderMatrix()
{
	// サイズの定義
	collFuncMatrix_.resize(ColliderType::COLLIDER_TYPES, std::vector<std::function<bool(std::weak_ptr<ColliderBase>, std::weak_ptr<ColliderBase>)>>(ColliderType::COLLIDER_TYPES));

	// 特定の組み合わせの関数を代入
	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::MODEL)][static_cast<int>(ColliderType::TYPE::CAPSULE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToCapsule(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::CAPSULE)][static_cast<int>(ColliderType::TYPE::MODEL)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToCapsule(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::MODEL)][static_cast<int>(ColliderType::TYPE::LINE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToLine(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::LINE)][static_cast<int>(ColliderType::TYPE::MODEL)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckModeToLine(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::BOX)][static_cast<int>(ColliderType::TYPE::CAPSULE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckCapsuleToBox(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::CAPSULE)][static_cast<int>(ColliderType::TYPE::BOX)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckCapsuleToBox(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::BOX)][static_cast<int>(ColliderType::TYPE::LINE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckLineToBox(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::LINE)][static_cast<int>(ColliderType::TYPE::BOX)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckLineToBox(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::LINE)][static_cast<int>(ColliderType::TYPE::CAPSULE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckCapsuleToLine(collA, collB);
		};

	collFuncMatrix_[static_cast<int>(ColliderType::TYPE::CAPSULE)][static_cast<int>(ColliderType::TYPE::LINE)] =
		[this](std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB) -> bool
		{
			return IsHitCheckCapsuleToLine(collA, collB);
		};
}

bool CollisionManager::IsHitCheckModeToCapsule(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderModel> collModel;
	std::weak_ptr<ColliderCapsule> collCapsule;

	// モデルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collB.lock()); }

	// カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collB.lock()); }

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

	// モデルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::MODEL) { collModel = std::dynamic_pointer_cast<ColliderModel>(collB.lock()); }

	// カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collB.lock()); }

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

bool CollisionManager::IsHitCheckCapsuleToBox(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderCapsule> collCapsule;
	std::weak_ptr<ColliderBox> collBox;

	// カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collB.lock()); }
	
	// ボックスコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::BOX) { collBox = std::dynamic_pointer_cast<ColliderBox>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::BOX) { collBox = std::dynamic_pointer_cast<ColliderBox>(collB.lock()); }

	// カプセルから必要な情報を取得
	auto& obb = collBox.lock()->GetObb();						// バウンディングボックスの情報
	VECTOR boxPos = collBox.lock()->GetPos();					// ボックスの位置
	VECTOR capTopPos = collCapsule.lock()->GetPosTop();			// カプセルの上部座標
	VECTOR capDownPos = collCapsule.lock()->GetPosDown();		// カプセルの下部座標
	float radius = collCapsule.lock()->GetRadius();				// 半径

	// 判定結果を返す
	return Utility3D::CheckHitBox_Capsule(obb, boxPos, capTopPos, capDownPos, radius);
}

bool CollisionManager::IsHitCheckCapsuleToLine(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderCapsule> collCapsule;
	std::weak_ptr<ColliderLine> collLine;

	// カプセルコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::CAPSULE) { collCapsule = std::dynamic_pointer_cast<ColliderCapsule>(collB.lock()); }

	// ラインコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collB.lock()); }

	// カプセルから必要な情報を取得
	VECTOR lineTopPos = collLine.lock()->GetLocalPosPointHead();// ラインの先端
	VECTOR lineEndPos = collLine.lock()->GetLocalPosPointEnd();	// ラインの末端
	VECTOR capTopPos = collCapsule.lock()->GetPosTop();			// カプセルの上部座標
	VECTOR capDownPos = collCapsule.lock()->GetPosDown();		// カプセルの下部座標
	float radius = collCapsule.lock()->GetRadius();				// 半径

	// 判定結果を返す
	if (Utility3D::CheckHitCapsuleToLine(capTopPos, capDownPos, radius, lineTopPos, lineEndPos))
	{
		return true;
	}

	return false;
}

bool CollisionManager::IsHitCheckLineToBox(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB)
{
	std::weak_ptr<ColliderLine> collLine;
	std::weak_ptr<ColliderBox> collBox;

	// ラインコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::LINE) { collLine = std::dynamic_pointer_cast<ColliderLine>(collB.lock()); }

	// ボックスコライダーの用意
	if (collA.lock()->GetType() == ColliderType::TYPE::BOX) { collBox = std::dynamic_pointer_cast<ColliderBox>(collA.lock()); }
	else if (collB.lock()->GetType() == ColliderType::TYPE::BOX) { collBox = std::dynamic_pointer_cast<ColliderBox>(collB.lock()); }

	// カプセルから必要な情報を取得
	auto& obb = collBox.lock()->GetObb();							// バウンディングボックスの情報
	VECTOR boxPos = collBox.lock()->GetPos();						// ボックスの位置
	VECTOR lineTopPos = collLine.lock()->GetLocalPosPointHead();	// ラインの先端座標
	VECTOR lineEndPos = collLine.lock()->GetLocalPosPointEnd();		// カプセルの末端座標

	// 判定結果を返す
	return Utility3D::CheckHitBox_Line(obb, boxPos, lineTopPos, lineEndPos);
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