#include "../../../Manager/Game/CollisionManager.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Collider/ColliderBase.h"
#include "StageGimmickObjectBase.h"

StageGimmickObjectBase::StageGimmickObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
	onHit_ = nullptr;
	anomalyCollider_ = nullptr;
	preTexture_ = -1;
	textureIndex_ = -1;
}

StageGimmickObjectBase::~StageGimmickObjectBase()
{
}

void StageGimmickObjectBase::Load()
{
	// 基底クラスの読み込み処理
	StageObjectBase::Load();

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);
}

void StageGimmickObjectBase::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	// 衝突後処理
	onHit_->OnHit(opponentCollider);
}

void StageGimmickObjectBase::SetAnomaly()
{
	// 自身のコライダーを複製して異変用に保持
	anomalyCollider_ = collider_->Clone();

	// タグの変更
	anomalyCollider_->ChangeTag(CollisionTags::TAG::ANOMALY);

	// コライダーの追加
	collMng_.Add(anomalyCollider_);
}

void StageGimmickObjectBase::Refresh()
{
	// コライダーの削除
	anomalyCollider_->SetDelete();

	// 中身を空にする
	anomalyCollider_ = nullptr;
}

void StageGimmickObjectBase::SetChangeColor(const int red, const int green, const int blue)
{
}

void StageGimmickObjectBase::ChangeTexture(const int textureHandle, const int textureIndex)
{
	// 変更するテクスチャ番号を設定
	textureIndex_ = textureIndex;

	// 変更前のテクスチャを取得
	preTexture_ = MV1GetTextureGraphHandle(transform_.modelId, textureIndex_);

	// テクスチャの変更
	MV1SetTextureGraphHandle(transform_.modelId, textureIndex_, textureHandle, true);
}

void StageGimmickObjectBase::DebugDraw()
{
	// 異変発生中異変コライダーの判定を可視化
	if (anomalyCollider_)
	{
		anomalyCollider_->DebugDraw();
	}

}
