#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectSnowNoise.h"

PostEffectSnowNoise::PostEffectSnowNoise()
{
	noiseStep_ = 0.0f;
}

PostEffectSnowNoise::~PostEffectSnowNoise()
{
}

void PostEffectSnowNoise::Init()
{
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("snowNoizeEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);	
	
	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y,BLUR_DISTANCE });

	// 変数の初期化
	noiseStep_ = 0.0f;

	// 親クラスの初期化処理
	PostEffectBase::Init();
}

void PostEffectSnowNoise::Draw()
{
	// ステップの更新
	noiseStep_ += scnMng_.GetDeltaTime();

	// バッファー設定
	material_->SetConstBuf(0, FLOAT4{ noiseStep_,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y,0.0f });

	// 親クラスの描画処理
	PostEffectBase::Draw();
}
