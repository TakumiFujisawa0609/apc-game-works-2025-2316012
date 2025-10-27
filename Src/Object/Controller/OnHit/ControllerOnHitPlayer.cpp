#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Utility/Utility3D.h"
#include "../../Actor/Character/Player.h"
#include "../../Collider/ColliderModel.h"
#include "../ControllerAnimation.h"
#include "ControllerOnHitPlayer.h"

ControllerOnHitPlayer::ControllerOnHitPlayer(Player& owner) : 
	owner_(owner)
{
	// 衝突物別関数の登録
	RegisterOnHit(CollisionTags::TAG::MAIN_STAGE , [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitMainStage(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::STAGE_GIMMICK, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitStageObject(opponentCollider); });
	RegisterOnHit(CollisionTags::TAG::ENEMY, [this](const std::weak_ptr<ColliderBase>& opponentCollider) { OnHitEnemy(opponentCollider); });
}

ControllerOnHitPlayer::~ControllerOnHitPlayer()
{
}

void ControllerOnHitPlayer::OnHitMainStage(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 座標取得
	Transform trans = Transform(owner_.GetTransform());
	VECTOR movedPos = trans.pos;

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
				trans.pos = movedPos;
				//trans.Update();
				owner_.SetPos(movedPos);
				//MV1RefreshCollInfo(trans.modelId);
				continue;
			}

			break;
		}
	}

	// 検出した地面ポリゴン情報の後始末
	collModel->CleaningPolyDim();

	// 最終的な位置を設定
	//owner_.SetPos(movedPos);

	// 衝突後処理の反映
	owner_.OnHitUpdate();
}

void ControllerOnHitPlayer::OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider)
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

	// 衝突後処理の反映
	owner_.OnHitUpdate();
}

void ControllerOnHitPlayer::OnHitEnemy(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 死亡状態へ変更
	owner_.ChangeState(Player::STATE::DEAD);
}