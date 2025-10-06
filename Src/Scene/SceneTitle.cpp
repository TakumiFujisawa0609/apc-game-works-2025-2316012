#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);
}

SceneTitle::~SceneTitle(void)
{
}

void SceneTitle::Load(void)
{
	//親クラスの読み込み
	SceneBase::Load();

	//リソースの設定
	auto& res = ResourceManager::GetInstance();
	testSprite_.handleId = res.GetHandle("testImage");
	se_ = res.GetHandle("testSe");
	bgm_ = res.GetHandle("testBgm");

	auto& snd = SoundManager::GetInstance();
	snd.Add(se_, SOUNDTYPE::SE);
	snd.Add(bgm_, SOUNDTYPE::BGM);
}

void SceneTitle::Init(void)
{
	//画像位置設定
	testSprite_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	SoundManager::GetInstance().Play(bgm_);
}

void SceneTitle::NormalUpdate(void)
{	
	// シーン遷移
	auto& snd = SoundManager::GetInstance();
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		snd.Stop(bgm_);
	}
}

void SceneTitle::NormalDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x0000ff,
		true
	);

	testSprite_.DrawRota();

	DrawFormatString(
		0, 0,
		0x000000,
		L"SceneTitle"
	);
}

void SceneTitle::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);
}

