#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Utility/Utility3D.h"
#include "../../Character/Player.h"
#include "../../Collider/ColliderModel.h"
#include "../ControllerAnimation.h"
#include "ControllerOnHitPlayer.h"

ControllerOnHitPlayer::ControllerOnHitPlayer(Player& owner) : 
	owner_(owner)
{
	// 衝突物別関数の登録
	RegisterOnHit(CollisionTags::MAIN_STAGE, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStage(opponentCollider); });
}

ControllerOnHitPlayer::~ControllerOnHitPlayer()
{
}

void ControllerOnHitPlayer::OnHitStage(const std::weak_ptr<ColliderBase>& opponentCollider)
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
				owner_.GetCapsuleTopPos(), owner_.GetCapsuleDownPos(), owner_.GetCapsuleRadius(),
				hit.Position[0], hit.Position[1], hit.Position[2]);

			if (pHit)
			{
				// 位置をずらす
				movedPos = VAdd(movedPos, VScale(hit.Normal, 1.0f));
				continue;
			}

			break;
		}
	}

	// 検出した地面ポリゴン情報の後始末
	collModel->CleaningPolyDim();

	// 重力方向
	constexpr VECTOR GRAVITY_DIR = Utility3D::DIR_D;		// 通常の方向
	constexpr VECTOR GRAVITY_DIR_UP = Utility3D::DIR_U;		// 反対方向

	// 座標取得
	constexpr float CHECK_POW = 10.0f;
	VECTOR gravHitPosUp = VAdd(movedPos, VScale(GRAVITY_DIR_UP, owner_.GetGravity()));
	gravHitPosUp = VAdd(gravHitPosUp, VScale(GRAVITY_DIR_UP, CHECK_POW * 2.0f));
	VECTOR gravHitPosDown = VAdd(movedPos, VScale(GRAVITY_DIR, CHECK_POW));

	// 地面との衝突
	auto hit = MV1CollCheck_Line(
		collModel->GetModel(), -1, gravHitPosUp, gravHitPosDown);

	// 地面と衝突している場合
	if (hit.HitFlag > 0 && VDot(GRAVITY_DIR, owner_.GetJumpPow()) > 0.9f)
	{
		// 衝突地点から、少し上に移動
		movedPos = VAdd(hit.HitPosition, VScale(GRAVITY_DIR_UP, 2.0f));

		// ジャンプリセット
		owner_.SetJumpPow(Utility3D::VECTOR_ZERO);	// ジャンプ力を初期化
		owner_.SetStepJump(0.0f);					// ステップを初期化

		// ジャンプ中の時
		if (owner_.IsJump())
		{
			// 着地モーション
			owner_.GetControllerAnimation().Play(Player::ANIM_JUMP, false, 29.0f, 45.0f, false, true);
		}

		// ジャンプ判定の設定
		owner_.SetIsJump(false);
	}

	// 最終的な位置を設定
	owner_.SetPos(movedPos);
}