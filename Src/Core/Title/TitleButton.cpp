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
	img_.pos = { Application::SCREEN_HALF_X - 476 / 2, 500 };
	img_.handleId = resMng_.GetHandle("pleaseSpaceKey");

	int Nps = resMng_.GetHandle("normalSpritePs");
	material_ = std::make_unique<PixelMaterial>(Nps, 1);
	material_->AddTextureBuf(img_.handleId);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	renderer_->SetPos(img_.pos);
	renderer_->SetSize({ 476, 48 });
	renderer_->MakeSquereVertex();
}