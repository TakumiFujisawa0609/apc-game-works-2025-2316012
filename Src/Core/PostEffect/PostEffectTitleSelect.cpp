#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectTitleSelect.h"

PostEffectTitleSelect::PostEffectTitleSelect()
{
	time_ = 0.0f;
}

PostEffectTitleSelect::~PostEffectTitleSelect()
{
}

void PostEffectTitleSelect::Init()
{
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("titleSelectEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectTitleSelect::Draw()
{
	// 時間
	time_ += scnMng_.GetDeltaTime();

	// 強さ(ズレ幅を調整)
	const float strength = 0.005f + (rand() % 100 / 10000.0f);

	// バッファー設定
	material_->SetConstBuf(0, FLOAT4{ time_,strength,Application::SCREEN_SIZE_Y,0.0f });

	// 親クラスの描画処理
	PostEffectBase::Draw();
}
