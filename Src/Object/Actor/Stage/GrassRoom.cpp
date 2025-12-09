#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/CollisionTags.h"
#include "../../../Utility/Utility3D.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Controller/Draw/ControllerDrawGrassRoom.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "Sub/Grass.h"
#include "GrassRoom.h"

GrassRoom::GrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageMain(key, mapParam, colliderParam)
{
	distance_ = 0.0f;
	startPos_ = Utility3D::VECTOR_ZERO;
	rectMax_ = Utility3D::VECTOR_ZERO;
	rectMin_ = Utility3D::VECTOR_ZERO;
}

GrassRoom::~GrassRoom()
{
}

void GrassRoom::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));	
	
	startPos_ = MV1GetFramePosition(transform_.modelId, 2);
	startPos_ = VScale(startPos_, -1);

	// 描画の設定
	draw_ = std::make_unique<ControllerDrawGrassRoom>(transform_.modelId, *this);
	draw_->Load();

	// 基底クラスの読み込み
	ActorBase::Load();

	// ダイナミックキャスト
	auto collider = std::dynamic_pointer_cast<ColliderBox>(collider_);

	// キャストが成功した場合
	if (collider)
	{
		// 参照を受け取る
		const ColliderBox& box = *collider;

		// ボックス
		rectMax_ = box.GetVecMax();
		rectMin_ = box.GetVecMin();
	}

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);
}

void GrassRoom::Init()
{
	StageMain::Init();
}

void GrassRoom::Update()
{
	// コライダーが生成されている場合
	if (anomalyCollider_)
	{
		// 距離の更新
		constexpr float SPEED = 3.0f;
		distance_ += SPEED;
	}
}

void GrassRoom::SetAnomaly()
{
	// モデルのコライダーを生成
	anomalyCollider_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::ANOMALY);

	// コライダーの追加
	collMng_.Add(anomalyCollider_);

	// テクスチャを追加
	draw_->SetTexture(resMng_.GetHandle("grassLand"));

	// 草の生成
	StageManager& stageMng = StageManager::GetInstance();
	for (int i = 0; i < CREATE_GRASS; i++)
	{
		stageMng.AddGrass(std::move(std::make_unique<Grass>(GetRespownGrassPos(), *this)));
	}
}

void GrassRoom::Refresh()
{
	StageGimmickObjectBase::Refresh();

	// テクスチャを解除
	draw_->SetTexture(-1);

	// 初期化
	distance_ = 0.0f;

	// 草の削除
	StageManager::GetInstance().DeleteGrass();

	// 効果音の停止
	SoundManager::GetInstance().StopSe(SoundType::SE::GRASS_LAND);
}

VECTOR GrassRoom::GetRespownGrassPos()
{
	VECTOR ret = Utility3D::VECTOR_ZERO;
	ret.x = rectMin_.x + 50 + GetRand(rectMax_.x - rectMin_.x - 1);
	ret.y = rectMin_.y;
	ret.z = rectMin_.z + 100 + GetRand(rectMax_.z - rectMin_.z - 1 - 100);
	return ret;
}