#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectFilmBurn.h"

PostEffectFilmBurn::PostEffectFilmBurn()
{
	step_ = 0.0f;
}

PostEffectFilmBurn::~PostEffectFilmBurn()
{
}

void PostEffectFilmBurn::Init()
{
	// 描画関係の生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("filmBurnEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,TIME, 0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());
	material_->AddTextureBuf(resMng_.GetHandle("filmBurnTexture"));

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectFilmBurn::Draw()
{
	// バッファーの設定
	material_->SetConstBuf(0, FLOAT4{ step_, TIME, 0.0f, 0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}