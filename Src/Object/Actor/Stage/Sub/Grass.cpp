#include "../../../../Manager/Common/ResourceManager.h"
#include "../../../../Render/ModelMaterial.h"
#include "../../../../Render/BillboardRenderer.h"
#include "../../../../Utility/Utility3D.h"
#include "../../../../Utility/UtilityCommon.h"
#include "../GrassRoom.h"
#include "Grass.h"

Grass::Grass(const VECTOR& pos, GrassRoom& parent) :
	pos_(pos),
	parent_(parent)
{
	alpha_ = 0.0f;
	isActive_ = false;
	startPos_ = Utility3D::VECTOR_ZERO;

	Init();
}

Grass::~Grass()
{
}

void Grass::Init()
{
	// リソース管理
	ResourceManager& res = ResourceManager::GetInstance();

	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(res.GetHandle("grassVs"), 0, res.GetHandle("grassPs"), 1);

	// レンダラー生成
	renderer_ = std::make_unique<BillboardRenderer>(*material_);

	// テクスチャの設定
	material_->SetTextureBuf(0, res.GetHandle("grass"));

	// アドレスの指定
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::BORDER);

	// バッファーの追加
	material_->AddConstBufPS(FLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	// サイズ定義
	size_ = { 35, 60 };

	// アルファ値
	alpha_ = 0.0f;

	// 頂点生成
	renderer_->MakeSquereVertex(pos_, size_);
}

void Grass::Update()
{
	// 距離を計算
	float distance = Utility3D::Distance(pos_, parent_.GetStartPos());

	// 距離に応じて活動状態を反映
	if (distance < parent_.GetDistance() && !CheckCameraViewClip(pos_))
	{
		constexpr float ALPHA_SPEED = 1.0f / 60.0f * 2.0f;

		// 設定
		isActive_ = true;

		// アルファ値を上昇
		alpha_ += ALPHA_SPEED;
		if (alpha_ > 1.0f) { alpha_ = 1.0f; }
	}
	else
	{
		// 初期化
		isActive_ = false;
		alpha_ = 0.0f;
	}


}

void Grass::Draw()
{
	if (isActive_)
	{
		material_->SetConstBufPS(0, FLOAT4(alpha_, 0.0f, 0.0f, 0.0f));

		renderer_->Draw();
	}
}