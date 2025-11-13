#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectSnowNoise.h"

PostEffectSnowNoise::PostEffectSnowNoise()
{
	time_ = 0.0f;
}

PostEffectSnowNoise::~PostEffectSnowNoise()
{
}

void PostEffectSnowNoise::Load()
{
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("snowNoizeEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void PostEffectSnowNoise::Init()
{
	time_ = 0.0f;
}

void PostEffectSnowNoise::Draw()
{
	// 時間
	time_ += scnMng_.GetDeltaTime();

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 親クラスの描画処理
	PostEffectBase::Draw();
}
