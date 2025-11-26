#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Utility/UtilityCommon.h"
#include "GameManual.h"

GameManual::GameManual()
{
}

GameManual::~GameManual()
{
}

void GameManual::Load()
{
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, FONT_THICK);
	text_.string = MANUAL_KEY;
	text_.pos = { 0, Application::SCREEN_SIZE_Y - FONT_SIZE * 8 };
	text_.fontHandle = font;
	text_.color = UtilityCommon::WHITE;
}

void GameManual::Init()
{
}

void GameManual::Update()
{
}

void GameManual::Draw()
{
	text_.Draw();
}
