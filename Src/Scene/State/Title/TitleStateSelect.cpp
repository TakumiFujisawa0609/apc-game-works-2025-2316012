#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "TitleStateSelect.h"

TitleStateSelect::TitleStateSelect(SceneTitle& parent) :
	TitleStateBase(parent)
{
}

TitleStateSelect::~TitleStateSelect()
{
}

void TitleStateSelect::Init()
{
}

void TitleStateSelect::Update()
{
}

void TitleStateSelect::Draw()
{
	// îwåi
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::RED,
		true
	);
}
