#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	//親クラスの読み込み
	SceneBase::Load();

	//リソースの設定
	auto& res = ResourceManager::GetInstance();
	testSprite_.handleId = res.Load(ResourceManager::SRC::TEST_IMG).handleId_;
	se_ = res.LoadSoundDuplicate(ResourceManager::SRC::TEST_SE);
	bgm_ = res.LoadSoundDuplicate(ResourceManager::SRC::TEST_BGM);

	auto& snd = SoundManager::GetInstance();
	snd.Add(se_, SOUNDTYPE::SE);
	snd.Add(bgm_, SOUNDTYPE::BGM);
}

void TitleScene::Init(void)
{
	//画像位置設定
	testSprite_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	SoundManager::GetInstance().Play(bgm_);
}

void TitleScene::NormalUpdate(void)
{	
	// シーン遷移
	auto& snd = SoundManager::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		snd.Stop(bgm_);
	}
	if (ins.IsTrgDown(KEY_INPUT_W))
	{
		snd.Play(se_);
	}
}

void TitleScene::NormalDraw(void)
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
		L"TitleScene"
	);
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

