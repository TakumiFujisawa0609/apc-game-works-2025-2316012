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
	material_->AddConstBufPS(FLOAT4(0.0f, GLITCH_STRENGTH, 0.0f, 0.0f));

	// サイズ定義
	size_ = GetRandSize();

	// 頂点生成
	renderer_->MakeSquereVertex(pos_, size_);

}

void Human::Update()
{
	step_ += scnMng_.GetDeltaTime();
}

void Human::Draw()
{
	if (isActive_)
	{
		// バッファー設定
		material_->SetConstBufPS(0, FLOAT4{ step_, GLITCH_STRENGTH, 0, 0 });

		// 描画
		renderer_->Draw();
	}
}

const Vector2& Human::GetRandSize() const
{
	Vector2 size;
	size.x =  SIZE_MIN_X + GetRand(SIZE_MAX_X - SIZE_MIN_X);
	size.y =  SIZE_MIN_Y + GetRand(SIZE_MAX_Y - SIZE_MIN_Y);
	return size;
}
