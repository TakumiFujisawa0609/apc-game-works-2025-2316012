//#include <random>
#include <cassert>
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Controller/Draw/ControllerDrawGrassRoom.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "Sub/Grass.h"
#include "GrassRoom.h"

//namespace
//{
//	// 乱数生成器
//	std::random_device rd;
//	std::mt19937 gen(rd());
//}

GrassRoom::GrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageMain(key, mapParam, colliderParam),
	stageMng_(StageManager::GetInstance())
{
	rectMax_ = {};
	rectMin_ = {};
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

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);
}

void GrassRoom::Init()
{
	// 基底クラスの初期化
	StageMain::Init();

	// ボックスコライダーへ変換
	auto boxPtr = std::dynamic_pointer_cast<ColliderBox>(collider_);

	// アサートで確認
	assert(boxPtr && "変換に失敗しました");

	// 参照の取得
	const ColliderBox& box = *boxPtr;

	// 草の生成範囲の取得
	rectMax_ = box.GetVecMax();
	rectMin_ = box.GetVecMin();
	rectMax_.y = rectMin_.y;
}

void GrassRoom::Update()
{
	// 活動状態の場合
	if (isActive_)
	{
		// 距離の更新
		distance_ += SPEED;
	}
}

void GrassRoom::Draw()
{
	draw_->Draw();
	//StageMain::Draw();
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

	// 草を一定数生成
	for (int i = 0; i < CREATE_NUM; i++)
	{
		// 草の追加
		stageMng_.AddGrass(std::move(std::make_unique<Grass>(SpawnRandomPos(), *this)));
	}
}

void GrassRoom::Refresh()
{
	// テクスチャを解除
	draw_->SetTexture(-1);

	// コライダー削除
	colliderModel_->SetDelete();

	// 草を削除
	stageMng_.DeleteAllGrass();

	// 初期化
	isActive_ = false;
	colliderModel_ = nullptr;
	distance_ = 0.0f;
}

const VECTOR& GrassRoom::GetStartPos() const
{
	// 異変を開始する位置をフレーム位置から取得
	VECTOR pos = VScale(MV1GetFramePosition(transform_.modelId, 2), -1);
	return pos;
}

VECTOR GrassRoom::SpawnRandomPos()
{
	VECTOR ret = Utility3D::VECTOR_ZERO;

	//// X座標の分布を設定
	//std::uniform_int_distribution<> distX(rectMin_.x, rectMax_.x);
	//std::uniform_int_distribution<> distZ(rectMin_.z, rectMax_.z);

	// 値の格納
	ret.x = rectMin_.x + GetRand(rectMax_.x - 1);
	ret.z = rectMin_.z + GetRand(rectMax_.z - 1);

	return ret;
}
