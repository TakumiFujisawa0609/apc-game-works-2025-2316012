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
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);
	
	// 変数の初期化
	se_ = -1;
	bgm_ = -1;
	logoMaterial_ = nullptr;
	logoRenderer_ = nullptr;
	keyMaterial_ = nullptr;
	keyRenderer_ = nullptr;
}

SceneTitle::~SceneTitle(void)
{
}

void SceneTitle::Load(void)
{
	//親クラスの読み込み
	SceneBase::Load();

	// ロゴ画像の設定
	int ps = resMng_.GetHandle("glitchPs");
	int Nps = resMng_.GetHandle("normalSpritePs");
	logoMaterial_ = std::make_unique<PixelMaterial>(ps, 2);				// マテリアルの生成
	logoMaterial_->AddTextureBuf(resMng_.GetHandle("titleLogo"));		// テクスチャの追加
	logoRenderer_ = std::make_unique<PixelRenderer>(*logoMaterial_);	// レンダラーの生成

	logoMaterial_->AddConstBuf({ 0.0f, 0.0f, 0.0f, 0.0f }); // 色の初期化
	logoMaterial_->AddConstBuf({ 0.0f, 0.0f, 0.0f, 0.0f }); // 色の初期化

	keyMaterial_ = std::make_unique<PixelMaterial>(Nps, 1);
	keyMaterial_->AddTextureBuf(resMng_.GetHandle("pleaseSpaceKey"));
	keyRenderer_ = std::make_unique<PixelRenderer>(*keyMaterial_);

	// サウンドの取得
	se_ = resMng_.GetHandle("testSe");
	bgm_ = resMng_.GetHandle("titleBgm");

	// サウンドの追加
	sndMng_.Add(se_, SOUNDTYPE::SE);
	sndMng_.Add(bgm_, SOUNDTYPE::BGM);
}

void SceneTitle::Init(void)
{
	// 画像位置設定

	logoRenderer_->SetPos({ 0, 260 });
	logoRenderer_->SetSize({ 640, 160 });
	logoRenderer_->MakeSquereVertex();	
	keyRenderer_->SetPos({ Application::SCREEN_HALF_X, 500 });
	keyRenderer_->SetSize({ 476, 48 });
	keyRenderer_->MakeSquereVertex();

	// BGMの再生
	sndMng_.Play(bgm_);
}

void SceneTitle::NormalUpdate(void)
{	
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		sndMng_.Stop(bgm_);
	}
}

void SceneTitle::NormalDraw(void)
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
	step_ + scnMng_.GetDeltaTime();
	float strength = sin(step_ * 5.0f) * 0.5f + 0.5f; // 強度を時間で0.0～1.0で変化させる
	float speed = 20.0f;

	logoMaterial_->SetConstBuf(1, { step_, strength, speed, 0.0f });

	// ロゴ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	logoRenderer_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// キー
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	keyRenderer_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);
}