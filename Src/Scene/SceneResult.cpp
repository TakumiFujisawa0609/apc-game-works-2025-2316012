#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "SceneResult.h"

SceneResult::SceneResult()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneResult::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneResult::LoadingDraw, this);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Load()
{
}

void SceneResult::Init()
{
}

void SceneResult::NormalUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void SceneResult::NormalDraw()
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
	DrawFormatString(0,0,UtilityCommon::BLACK, L"死んでしまうとは情けない");
}

void SceneResult::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneResult::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneResult::NormalDraw, this);
}
