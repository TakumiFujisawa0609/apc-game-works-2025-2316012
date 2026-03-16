#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectFireTransition.h"

PostEffectFireTransition::PostEffectFireTransition(const Json& param):
	PostEffectBase(param),
	BLUR_LEVEL(param["blurLevel"])
{	
	time_ = 0;
}

PostEffectFireTransition::~PostEffectFireTransition()
{
}

void PostEffectFireTransition::Init()
{
	// 描画関係の生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("fireTransitionEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f, BLUR_LEVEL,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());
	material_->AddTextureBuf(subTexture_);
	material_->AddTextureBuf(resMng_.GetHandle("fireTransitionTexture"));

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectFireTransition::Draw()
{
	// 時間の更新
	time_ += scnMng_.GetDeltaTime();

	// サブテクスチャの設定
	material_->SetTextureBuf(1, subTexture_);

	// バッファーの設定
	material_->SetConstBuf(0, FLOAT4{ time_, step_, BLUR_LEVEL, 0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}