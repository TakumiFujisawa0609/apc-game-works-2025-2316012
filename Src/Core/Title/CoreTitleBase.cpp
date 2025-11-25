#include "../../Render/PixelMaterial.h"
#include "../../Render/PixelRenderer.h"
#include "CoreTitleBase.h"

CoreTitleBase::CoreTitleBase()
{
	isEffect_ = false;
	material_ = nullptr;
	renderer_ = nullptr;
}

CoreTitleBase::~CoreTitleBase()
{
}

void CoreTitleBase::Draw()
{
	if (isEffect_)
	{
		renderer_->Draw();
	}
	else
	{
		// ’Êí•`‰æ
		img_.Draw();
	}
}
