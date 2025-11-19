#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Resource/FontManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../SceneTitle.h"
#include "TitleStateExplanation.h"

TitleStateExplanation::TitleStateExplanation(SceneTitle& parent) :
	TitleStateBase(parent)
{
}

TitleStateExplanation::~TitleStateExplanation()
{
}

void TitleStateExplanation::Init()
{
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 16, 0);

	skipText_.color = UtilityCommon::BLACK;
	skipText_.pos = { Application::SCREEN_SIZE_X - 100, Application::SCREEN_SIZE_Y - 20 };
	skipText_.fontHandle = font;
	skipText_.string = L"スキップ";
}

void TitleStateExplanation::Update()
{
	// スキップの入力がされた場合
	if (inputMng_.IsTrgDown(InputManager::TYPE::EXPLANTION_SKIP))
	{
		// シーン遷移
		parent_.ChangeState(SceneTitle::STATE::SELECT);
		return;
	}
}

void TitleStateExplanation::Draw()
{
	// 背景
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::WHITE,
		true
	);

	// テキストの描画
	skipText_.Draw();
}
