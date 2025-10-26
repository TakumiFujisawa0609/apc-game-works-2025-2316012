#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Actor/Character/CharacterBase.h"
#include "../../Collider/ColliderModel.h"
#include "ControllerOnHitGravity.h"

ControllerOnHitGravity::ControllerOnHitGravity(CharacterBase& owner) :
	owner_(owner)
{
	// 衝突物別関数の登録
	RegisterOnHit(CollisionTags::TAG::MAIN_STAGE, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStage(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::STAGE_GIMMICK, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStage(opponentCollider); });
}

ControllerOnHitGravity::~ControllerOnHitGravity()
{
}

void ControllerOnHitGravity::OnHitStage(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 座標取得
	VECTOR movedPos = owner_.GetTransform().pos;

	// モデル用コライダーへ変換
	auto collModel = std::dynamic_pointer_cast<ColliderModel>(opponentCollider.lock());

	// 衝突結果を取得
	auto& hit = collModel->GetCollResultPoly();

	// 地面と衝突している場合
	if (hit.HitFlag > 0 && VDot(Utility3D::DIR_U, owner_.GetJumpPow()) > 0.9f)
	{
		// 衝突地点から、少し上に移動
		movedPos = VAdd(hit.HitPosition, VScale(Utility3D::DIR_U, 2.0f));

		// ジャンプリセット
		owner_.SetJumpPow(Utility3D::VECTOR_ZERO);	// ジャンプ力を初期化
	}

	// プレイヤーの重力制限
	if (movedPos.y < 0.0f)
	{
		movedPos.y = 0.0f;
	}

	// 最終的な位置を設定
	owner_.SetPos(movedPos);
}
