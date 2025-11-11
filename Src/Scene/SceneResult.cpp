#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/System/ScoreManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "SceneResult.h"

// スコアマネージャーの終了状態の省略形
using END = ScoreManager::END_STATE;

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
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 56, 0);

	// テキスト設定
	text_.fontHandle = font;
	text_.pos = Vector2{ Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	text_.color = UtilityCommon::BLACK;

	// ゲーム結果に合わせてテキストを変更
	ScoreManager& score = ScoreManager::GetInstance();
	if (score.GetEndState() == END::CLEAR)
	{
		text_.string = L"今回の賃金は%d円です";
		text_.data1 = score.GetScore();
	}
	else
	{
		text_.string = L"今回の賃金は0円です";
	}
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

	// テキスト描画
	text_.DrawFormatCenter();
}

void SceneResult::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneResult::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneResult::NormalDraw, this);
}
