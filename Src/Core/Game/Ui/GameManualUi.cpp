#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Resource/FontManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "GameManualUi.h"

GameManualUi::GameManualUi()
{
}

GameManualUi::~GameManualUi()
{
}

void GameManualUi::Load()
{
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontDot"), FONT_SIZE, FONT_THICK);
	text_.string = MANUAL_KEY;
	text_.pos = { 0, Application::SCREEN_SIZE_Y - FONT_SIZE * 6 };
	text_.fontHandle = font;
	text_.color = UtilityCommon::WHITE;
}

void GameManualUi::Init()
{
}

void GameManualUi::Update()
{
}

void GameManualUi::Draw()
{
	text_.Draw();
}
