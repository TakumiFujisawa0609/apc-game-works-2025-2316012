#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
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
	
	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y,BLUR_DISTANCE });

	Init();
}

void PostEffectSnowNoise::Init()
{
	time_ = 0.0f;

	// 親クラスの初期化処理
	PostEffectBase::Init();
}

void PostEffectSnowNoise::Draw()
{
	// 時間
	time_ += scnMng_.GetDeltaTime();

	// バッファー設定
	material_->SetConstBuf(0, FLOAT4{ time_,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y,BLUR_DISTANCE });

	// 親クラスの描画処理
	PostEffectBase::Draw();
}
