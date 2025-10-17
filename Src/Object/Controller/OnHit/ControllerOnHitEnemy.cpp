#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Utility/Utility3D.h"
#include "../../Actor/Character/Player.h"
#include "../../Collider/ColliderModel.h"
#include "../../Actor/Character/Enemy.h"
#include "../Action/ControllerActionEnemy.h"
#include "ControllerOnHitEnemy.h"

ControllerOnHitEnemy::ControllerOnHitEnemy(Enemy& owner) :
	owner_(owner),
	actionCtrl_(*owner.GetActionEnemy())
{
	// 衝突物別処理の登録
	RegisterOnHit(CollisionTags::TAG::MAIN_STAGE, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitMainStage(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::STAGE_GIMMICK, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::PLAYER, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitPlayer(opponentCollider); });
}

ControllerOnHitEnemy::~ControllerOnHitEnemy()
{
}

void ControllerOnHitEnemy::OnHitMainStage(const std::weak_ptr<ColliderBase>& opponentCollider)
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

	// 最終的な位置を設定
	owner_.SetPos(movedPos);
}

void ControllerOnHitEnemy::OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 移動量
	constexpr float MOVE_POW = 0.1f;

	// 移動前の座標を取得
	VECTOR pos = owner_.GetPrePos();

	// 進行方向を取得
	VECTOR dir = owner_.GetMoveDir();

	// 進行方向の反対向きにして移動量を計算
	VECTOR movePow = VScale(dir, -MOVE_POW);
	movePow.y = 0;	// Y方向は無視する

	// 進行方向の反対向きに少し押す
	pos = VAdd(pos, movePow);

	// 座標の格納
	owner_.SetPos(pos);
}

void ControllerOnHitEnemy::OnHitPlayer(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 攻撃状態に変更してジャンプスケアを実行
	actionCtrl_.ChangeState(ControllerActionEnemy::STATE::ACTION);
}