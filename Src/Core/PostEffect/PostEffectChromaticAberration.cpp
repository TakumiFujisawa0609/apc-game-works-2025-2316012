#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectChromaticAberration.h"

PostEffectChromaticAberration::PostEffectChromaticAberration(const Json& param) :
	PostEffectBase(param),
	COLOR_OFFSET({ param["colorOffset"]["x"], param["colorOffset"]["y"]})
{
}

PostEffectChromaticAberration::~PostEffectChromaticAberration()
{
}

void PostEffectChromaticAberration::Init()
{
	// 描画関係の生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("chromaticAberrationEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ COLOR_OFFSET.x, COLOR_OFFSET.y, 0.0f, 0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	PostEffectBase::Init();
}

void PostEffectChromaticAberration::Draw()
{
	PostEffectBase::Draw();
}