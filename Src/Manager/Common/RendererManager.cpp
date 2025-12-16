#include <DxLib.h>
#include "../../Application.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/PixelRenderer.h"
#include "../../Render/BillboardRenderer.h"
#include "RendererManager.h"

void RendererManager::Init()
{
	// テクスチャの取得
	texturesMap_[TEXTURE_TYPE::MAIN] = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	texturesMap_[TEXTURE_TYPE::SHADOW] = MakeScreen(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, false);
	texturesMap_[TEXTURE_TYPE::POST_EFFECT] = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);

	// 影の初期化処理
}

void RendererManager::Draw()
{
	// モデルレンダラー描画
	for (const auto& modelRenderer : modelRenderers_)
	{
		modelRenderer->Draw();
	}

	// ビルボードレンダラー描画
	for (const auto& billboardRenderer : billboardRenderers_)
	{
		billboardRenderer->Draw();
	}

	// ピクセルレンダラー描画
	for (const auto& pixelRenderer : pixelRenderers_)
	{
		pixelRenderer->Draw();
	}
}

void RendererManager::Release()
{
	// 中身の削除
	SceneChangeRelease();

	// テクスチャの削除
	for (const auto& texturePair : texturesMap_)
	{
		DeleteGraph(texturePair.second);
	}
}

void RendererManager::SceneChangeRelease()
{
	pixelRenderers_.clear();
	modelRenderers_.clear();
	billboardRenderers_.clear();
}

void RendererManager::ChangePostEffect(const POST_EFFECT_TYPE postEffectType)
{
	postEffectType_ = postEffectType;
}

void RendererManager::AddPixelRenderer(const std::shared_ptr<PixelRenderer> pixelRenderer)
{
	pixelRenderers_.emplace_back(pixelRenderer);
}

void RendererManager::AddModelRenderer(const std::shared_ptr<ModelRenderer> modelRenderer)
{
	modelRenderers_.emplace_back(modelRenderer);
}

void RendererManager::AddBillboardRenderer(const std::shared_ptr<BillboardRenderer> billboardRenderer)
{
	billboardRenderers_.emplace_back(billboardRenderer);
}

void RendererManager::AddPostEffectRenderer(const POST_EFFECT_TYPE type, const std::shared_ptr<PixelRenderer> pixelRenderer)
{
	postEffectRenderersMap_.emplace(type, pixelRenderer);
}

const int RendererManager::GetTexture(const TEXTURE_TYPE textureType) const
{
    return texturesMap_.at(textureType);
}

RendererManager::RendererManager()
{
}

RendererManager::~RendererManager()
{
}
