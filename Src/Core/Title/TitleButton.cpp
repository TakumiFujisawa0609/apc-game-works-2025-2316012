#include "../../Application.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Render/PixelMaterial.h"
#include "../../Render/PixelRenderer.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "TitleButton.h"

TitleButton::TitleButton()
{
}

TitleButton::~TitleButton()
{
}

void TitleButton::Init()
{
	// ÉçÉSâÊëúÇÃê›íË
	int Nps = resMng_.GetHandle("normalSpritePs");
	material_ = std::make_unique<PixelMaterial>(Nps, 1);
	material_->AddTextureBuf(resMng_.GetHandle("pleaseSpaceKey"));
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	renderer_->SetPos({ Application::SCREEN_HALF_X - 476 / 2, 500 });
	renderer_->SetSize({ 476, 48 });
	renderer_->MakeSquereVertex();
}

void TitleButton::Update()
{
}

void TitleButton::Draw()
{
	// ÉLÅ[
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
