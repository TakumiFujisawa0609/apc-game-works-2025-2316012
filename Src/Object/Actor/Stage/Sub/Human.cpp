#include "../../../../Manager/Common/SceneManager.h"
#include "../../../../Manager/Common/ResourceManager.h"
#include "../../Render/BillboardRenderer.h"
#include "../../Render/ModelMaterial.h"
#include "Human.h"

Human::Human(const VECTOR& pos) :
	BillboardObjectBase(pos)
{

}

Human::~Human()
{
}

void Human::Init()
{
	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("humanVs"), 0, resMng_.GetHandle("humanPs"), 1);

	// レンダラー生成
	renderer_ = std::make_unique<BillboardRenderer>(*material_);

	// テクスチャの設定
	material_->SetTextureBuf(0, resMng_.GetHandle("human"));

	// アドレスの指定
	material_->SetTextureAddress(ModelMaterial::TEXADDRESS::BORDER);

	// バッファーの追加
	material_->AddConstBufPS(FLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	// サイズ定義
	size_ = { 300, 400 };

	// 頂点生成
	renderer_->MakeSquereVertex(pos_, size_);
}

void Human::Update()
{
}

void Human::Draw()
{
	if (isActive_)
	{
		renderer_->Draw();
	}
}
