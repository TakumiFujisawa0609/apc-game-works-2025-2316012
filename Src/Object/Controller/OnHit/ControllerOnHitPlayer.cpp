#include "../../Character/Player.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/CollisionTag.h"
#include "../../../Utility/Utility3D.h"
#include "../ControllerAnimation.h"
#include "ControllerOnHitPlayer.h"

ControllerOnHitPlayer::ControllerOnHitPlayer(Player& owner) : 
	owner_(owner)
{
	// 衝突物別関数の登録
	RegisterOnHit(COLLISION_TAG::STAGE, [this](std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStage(opponentCollider); });
}

ControllerOnHitPlayer::~ControllerOnHitPlayer()
{
}

void ControllerOnHitPlayer::OnHitStage(std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 座標取得
	VECTOR movedPos = owner_.GetTransform().pos;

	// モデル用コライダーへ変換
	auto collModel = std::dynamic_pointer_cast<ColliderModel>(opponentCollider.lock());

	// 衝突結果を取得
	auto& hits = collModel->GetCollResultPolyDim();

	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];

		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
		{
			int pHit = HitCheck_Capsule_Triangle(
				owner_.GetCapsuleTopPos(), owner_.GetCapsuleTopPos(), owner_.GetCapsuleRadius(),
				hit.Position[0], hit.Position[1], hit.Position[2]);

			if (pHit)
			{
				movedPos = VAdd(movedPos, VScale(hit.Normal, 1.0f));

				// 移動させる
				owner_.SetPos(movedPos);
				continue;
			}

			break;
		}
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);
}

void ControllerOnHitPlayer::OnHitGravity(std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 座標取得
	VECTOR movedPos = owner_.GetTransform().pos;

	// モデル用コライダーへ変換
	auto collModel = std::dynamic_pointer_cast<ColliderModel>(opponentCollider.lock());

	// 衝突後の情報を取得
	auto hit = collModel->GetCollResultPoly();

	if (hit.HitFlag > 0 && VDot(Utility3D::DIR_D, owner_.GetJumpPow()) > 0.9f)
	{

		// 衝突地点から、少し上に移動
		movedPos = VAdd(hit.HitPosition, VScale(Utility3D::DIR_U, 2.0f));

		// ジャンプリセット
		owner_.SetJumpPow(Utility3D::VECTOR_ZERO);

		if (owner_.IsJump())
		{
			// 着地モーション
			owner_.GetControllerAnimation().Play(Player::ANIM_JUMP, false, 29.0f, 45.0f, false, true);
		}

		// ジャンプ判定の設定
		owner_.SetIsJump(false);
	}
}