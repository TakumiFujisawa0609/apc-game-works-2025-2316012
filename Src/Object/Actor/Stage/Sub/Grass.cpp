#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/Resource/ResourceManager.h"
#include "../../../../Render/ModelMaterial.h"
#include "../../../../Render/BillboardRenderer.h"
#include "../../../../Utility/Utility3D.h"
#include "../GrassRoom.h"
#include "Grass.h"

Grass::Grass(const VECTOR& pos, const GrassRoom& grassRoom) :
	pos_(pos),
	grassRoom_(grassRoom),
	scnMng_(SceneManager::GetInstance())
{
	ResourceManager& res = ResourceManager::GetInstance();

	// 初期化
	step_ = 0.0f;
	size_ = {};

	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(res.GetHandle("grassVs"), 0, res.GetHandle("grassPs"), 1);

	// テクスチャの追加
	material_->SetTextureBuf(7, res.GetHandle("grass"));

	// レンダラー生成
	renderer_ = std::make_unique<BillboardRenderer>(*material_);
}

Grass::~Grass()
{
}

void Grass::Init()
{
	// 初期化
	step_ = 0.0f;
	
	// サイズ定義
	size_ = SIZE;

	// バッファーの追加
	material_->AddConstBufPS(FLOAT4{ step_,AMPLITUDE, 0.0f, 0.0f });

	// 矩形の生成
	renderer_->MakeSquereVertex(pos_, size_);

	// 活動状態をアクティブにする
	isActive_ = true;
}

void Grass::Update()
{
	float distance = grassRoom_.GetDistance();
	VECTOR startPos = grassRoom_.GetStartPos();

	float distance2 = Utility3D::Distance(grassRoom_.GetStartPos(), pos_);

	// 開始位置からの距離に応じて活動状態を変更
	isActive_ = grassRoom_.GetDistance() >= Utility3D::Distance(grassRoom_.GetStartPos(), pos_);
}

void Grass::Draw()
{
	// 活動状態の場合
	if (isActive_)
	{
		// ステップ更新
		step_ += scnMng_.GetDeltaTime();

		// マテリアル設定
		material_->SetConstBufPS(0, FLOAT4{ step_,AMPLITUDE, 0.0f, 0.0f });

		// 描画
		renderer_->Draw();
	}
}
