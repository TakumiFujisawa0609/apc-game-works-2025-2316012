#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "SceneTips.h"

SceneTips::SceneTips()
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneTips::NormalUpdate, this);

	//描画関数のセット
	drawFunc_ = std::bind(&SceneTips::NormalDraw, this);
}

SceneTips::~SceneTips()
{
}

void SceneTips::NormalUpdate()
{
	// シーンを戻す
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		scnMng_.PopScene();
		return;
	}
}

void SceneTips::NormalDraw()
{
	// 仮描画
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::BLACK, TRUE);
}
