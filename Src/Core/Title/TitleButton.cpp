#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
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
	// ƒƒS‰æ‘œ‚Ìİ’è
	img_.pos = { POS_X,POS_Y };
	img_.handleId = resMng_.GetHandle("pleaseSpaceKey");

	int Nps = resMng_.GetHandle("normalSpritePs");
	material_ = std::make_unique<PixelMaterial>(Nps, BUFFER_SIZE);
	material_->AddTextureBuf(img_.handleId);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	renderer_->SetPos(img_.pos);
	renderer_->SetSize({ SIZE_X, SIZE_Y });
	renderer_->MakeSquereVertex();
}