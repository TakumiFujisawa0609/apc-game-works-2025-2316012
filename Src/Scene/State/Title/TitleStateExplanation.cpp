#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Utility/UtilityCommon.h"
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
}

void TitleStateExplanation::Update()
{
	
}

void TitleStateExplanation::Draw()
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
