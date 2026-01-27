#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectTvError.h"

PostEffectTvError::PostEffectTvError()
{
}

PostEffectTvError::~PostEffectTvError()
{
}

void PostEffectTvError::Init()
{
	// 描画関係の生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("tvErrorEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f, 0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(resMng_.GetHandle("textureTvError"));

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectTvError::Draw()
{
	step_ += scnMng_.GetDeltaTime();

	// バッファーの設定
	material_->SetConstBuf(0, FLOAT4{ step_, STRNGTH, 0.0f, 0.0f });

	// 描画
	renderer_->Draw();
}