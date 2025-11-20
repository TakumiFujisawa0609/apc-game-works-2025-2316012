#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../../Core/Title/TitleButton.h"
#include "../../../Core/Title/TitleLogo.h"
#include "../../../Core/PostEffect/PostEffectRipples.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../SceneTitle.h"
#include "TitleStateMain.h"

TitleStateMain::TitleStateMain(SceneTitle& parent) :
	TitleStateBase(parent)
{
	effectScreen_ = -1;
	screenAlpha_ = 0.0f;
	step_ = 0.0f;
	logo_ = nullptr;
	button_ = nullptr;
}

TitleStateMain::~TitleStateMain()
{
	DeleteGraph(effectScreen_);
}

void TitleStateMain::Init()
{
	// ロゴの生成
	logo_ = std::make_unique<TitleLogo>();
	logo_->Load();
	logo_->Init();

	// ボタン
	button_ = std::make_unique<TitleButton>();
	button_->Init();

	// ポストエフェクト
	ripples_ = std::make_unique<PostEffectRipples>();
	ripples_->Load();
	ripples_->Init();

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::TITLE);

	// 初期更新処理
	update_ = std::bind(&TitleStateMain::UpdateWait, this);

	// 初期透過率
	screenAlpha_ = UtilityCommon::ALPHA_MAX;

	// スクリーンの生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void TitleStateMain::Update()
{
	// ロゴの更新
	logo_->Update();

	// ボタンの更新
	button_->Update();

	// 一部状態別処理
	update_();
}

void TitleStateMain::Draw()
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)screenAlpha_);

	// ロゴ描画
	logo_->Draw();

	// ボタン描画
	button_->Draw();

	//スクリーンの設定
	SetDrawScreen(effectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// ポストエフェクト描画
	ripples_->Draw();

	// メインに戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// 描画
	DrawGraph(0, 0, effectScreen_, false);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleStateMain::UpdateWait()
{
	// ゲーム開始の入力をした場合
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 更新処理の変更
		update_ = std::bind(&TitleStateMain::UpdateEffect, this);

		// エフェクトの再生
		ripples_->SetStart();

		// BGMの停止
		sndMng_.StopBgm(SoundType::BGM::TITLE);

		// 効果音の再生
		sndMng_.PlaySe(SoundType::SE::GAME_START);
	}
}

void TitleStateMain::UpdateEffect()
{
	step_ += scnMng_.GetDeltaTime();
	constexpr float ALPHA_RATE = 1.39f;
	screenAlpha_ -= ALPHA_RATE;
	if (screenAlpha_ < 0.0f)
	{
		// 状態遷移
		parent_.ChangeState(SceneTitle::STATE::EXPLANATION);
	}
}
