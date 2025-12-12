#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Render/PixelMaterial.h"
#include "../../Render/PixelRenderer.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "TitleLogo.h"

TitleLogo::TitleLogo()
{
	logo_ = -1;
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::Init()
{
	logo_ = resMng_.GetHandle("titleLogo");
}

void TitleLogo::Draw()
{
	// í èÌï`âÊ
	DrawGraph(POS_X, POS_Y, logo_, true);
}