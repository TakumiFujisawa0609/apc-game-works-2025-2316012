#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectNightView.h"

PostEffectNightView::PostEffectNightView()
{
}

PostEffectNightView::~PostEffectNightView()
{
}

void PostEffectNightView::Load()
{
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("nightViewEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void PostEffectNightView::Init()
{
	// バッファー追加
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 親クラスの初期化処理
	PostEffectBase::Init();
}
