#include "../Application.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectBase.h"

PostEffectBase::PostEffectBase()
{
	material_ = nullptr;
	renderer_ = nullptr;
}

PostEffectBase::~PostEffectBase()
{

}

void PostEffectBase::Load()
{
}

void PostEffectBase::Init()
{
	//‹éŒ`‚ÌÝ’è
	renderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y));
}

void PostEffectBase::Draw()
{
	renderer_->Draw();
}