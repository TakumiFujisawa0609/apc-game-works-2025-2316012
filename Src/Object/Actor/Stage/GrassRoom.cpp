#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Controller/Draw/ControllerDrawGrassRoom.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "GrassRoom.h"

GrassRoom::GrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageMain(key, mapParam, colliderParam)
{
	distance_ = 0.0f;
}

GrassRoom::~GrassRoom()
{
}

void GrassRoom::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// 描画の設定
	draw_ = std::make_unique<ControllerDrawGrassRoom>(transform_.modelId, *this);
	draw_->Load();

	// 基底クラスの読み込み
	ActorBase::Load();
}

void GrassRoom::Init()
{
}

void GrassRoom::Update()
{
	// 活動状態の場合
	if (isActive_)
	{
		// 距離の更新
		constexpr float SPEED = 1000.0f;
		const float deltaTime = scnMng_.GetDeltaTime();
		distance_ += deltaTime * deltaTime * SPEED;
	}
}

void GrassRoom::SetAnomaly()
{
	// テクスチャを追加
	draw_->SetTexture(resMng_.GetHandle("grassLand"));

	// 報告用にコライダーを設定
	colliderModel_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::DECO_GIMMICK);
	collMng_.Add(colliderModel_);

	// 活動状態の変更
	isActive_ = true;
}

void GrassRoom::Refresh()
{
	// テクスチャを解除
	draw_->SetTexture(-1);

	// コライダー削除
	colliderModel_->SetDelete();

	// 初期化
	isActive_ = false;
	colliderModel_ = nullptr;
	distance_ = 0.0f;
}
