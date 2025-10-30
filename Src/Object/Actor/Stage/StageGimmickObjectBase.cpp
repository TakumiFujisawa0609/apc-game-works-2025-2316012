#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "StageGimmickObjectBase.h"

StageGimmickObjectBase::StageGimmickObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
	onHit_ = nullptr;
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

void StageGimmickObjectBase::Refresh()
{
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
