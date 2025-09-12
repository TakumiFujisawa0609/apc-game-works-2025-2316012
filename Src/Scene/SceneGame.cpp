#include "SceneGame.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "ScenePause.h"

SceneGame::SceneGame(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);
}

SceneGame::~SceneGame(void)
{
	//インスタンスの削除
}

void SceneGame::Load(void)
{
	//親クラスの読み込み
	SceneBase::Load();

	//ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();
}

void SceneGame::Init(void)
{
}

void SceneGame::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	//デバッグ処理
	DebagUpdate();
}

void SceneGame::NormalDraw(void)
{
	//デバッグ処理
	DebagDraw();
}

void SceneGame::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);
}

void SceneGame::DebagUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	frame_++;
}

void SceneGame::DebagDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x00ff00,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"SceneGame"
	);

	constexpr float r = 40.0f;
	float angle = DX_PI_F * 2.0f * static_cast<float>(frame_ % 360) / 60.0f;

	//円運動を描画
	DrawCircleAA(
		320+cos(angle) * r, 
		240+sin(angle) * r,
		r, 
		32, 
		0xff8888, 
		true);
}
