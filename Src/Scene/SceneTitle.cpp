#include <string>
#include <cmath>
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
#include "../Core/Title/TitleLogo.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);
	
	// 変数の初期化
	logo_ = nullptr;
	keyMaterial_ = nullptr;
	keyRenderer_ = nullptr;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Load()
{
	//親クラスの読み込み
	SceneBase::Load();

	// ロゴの生成
	logo_ = std::make_unique<TitleLogo>();
	logo_->Load();

	// ロゴ画像の設定
	int Nps = resMng_.GetHandle("normalSpritePs");
	keyMaterial_ = std::make_unique<PixelMaterial>(Nps, 1);
	keyMaterial_->AddTextureBuf(resMng_.GetHandle("pleaseSpaceKey"));
	keyRenderer_ = std::make_unique<PixelRenderer>(*keyMaterial_);
}

void SceneTitle::Init()
{
	keyRenderer_->SetPos({ Application::SCREEN_HALF_X - 476 / 2, 500 });
	keyRenderer_->SetSize({ 476, 48 });
	keyRenderer_->MakeSquereVertex();

	// ロゴ初期化
	logo_->Init();

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::TITLE);
}

void SceneTitle::NormalUpdate()
{	
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		sndMng_.StopBgm(SoundType::BGM::TITLE);
		sndMng_.PlaySe(SoundType::SE::GAME_START);
	}

	// ロゴ更新
	logo_->Update();
}

void SceneTitle::NormalDraw()
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

	// ロゴ描画
	logo_->Draw();

	// キー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	keyRenderer_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::ChangeNormal()
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);
}