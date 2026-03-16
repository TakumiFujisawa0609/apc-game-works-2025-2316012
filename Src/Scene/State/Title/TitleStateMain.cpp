#include "../../../Application.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/InputManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/FontManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Core/Title/TitleButton.h"
#include "../../../Core/Title/TitleOrb.h"
#include "../../../Core/Title/TitleLogo.h"
#include "../../../Core/PostEffect/PostEffectRipples.h"
#include "../../../Core/Common/GlitchScreen.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../SceneTitle.h"
#include "../../SceneExplanation.h"
#include "../../SceneTips.h"
#include "TitleStateMain.h"

TitleStateMain::TitleStateMain(SceneTitle& parent, const Json& param) :
	TitleStateBase(parent),
	OPERATION_MESSAGE_KEY(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["operationMessageKey"].get<std::string>()))),
	OPERATION_MESSAGE_PAD(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["operationMessagePad"].get<std::string>()))),
	SCREEN_OPERATION_HEIGHT(param["screenOperationHeight"].get<int>()),
	FONT_SIZE(param["fontSize"].get<int>()),
	FONT_SIZE_OPERATION(param["fontSizeOperation"].get<int>()),
	FONT_THICK(param["fontThick"].get<int>()),
	FONT_THICK_OPERATION(param["fontThickOperation"].get<int>()),
	BOX_PADDING_X(param["boxPaddingX"].get<int>()),
	BOX_PADDING_Y(param["boxPaddingY"].get<int>()),
	FADE_ALPHA_RATE(param["fadeAlphaRate"].get<float>()),
	EFFECT_ALPHA_RATE(param["effectAlphaRate"].get<float>()),
	POST_EFFECT_PARAM(param["ripples"])
{
	// テキストの登録
	const auto& textArray = param["menuStrings"].get<std::vector<std::string>>();
	for (auto& text : textArray)
	{
		std::wstring wstr = UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(text));
		textList_.push_back(wstr);
	}

	effectScreen_ = -1;
	menuIndex_ = -1;
	screenAlpha_ = 0.0f;
	step_ = 0.0f;
	logo_ = nullptr;
	orb_ = nullptr;
	button_ = nullptr;
	state_ = STATE::NONE;
	nextState_ = STATE::NONE;

	// 状態遷移時の処理を登録
	stateChangeMap_.emplace(STATE::NONE, std::bind(&TitleStateMain::ChangeStateNone, this));
	stateChangeMap_.emplace(STATE::WAIT, std::bind(&TitleStateMain::ChangeStateWait, this));
	stateChangeMap_.emplace(STATE::SELECT, std::bind(&TitleStateMain::ChangeStateSelect, this));
	stateChangeMap_.emplace(STATE::FADE_IN, std::bind(&TitleStateMain::ChangeStateFadeIn, this));
	stateChangeMap_.emplace(STATE::FADE_OUT, std::bind(&TitleStateMain::ChangeStateFadeOut, this));
	stateChangeMap_.emplace(STATE::EFFECT, std::bind(&TitleStateMain::ChangeStateEffect, this));

	// メニュー時の処理を登録
	menuChangeMap_.emplace(MENU_TYPE::START, std::bind(&TitleStateMain::ChangeMenuStart, this));
	menuChangeMap_.emplace(MENU_TYPE::EXPLANATION, std::bind(&TitleStateMain::ChangeMenuExplanation, this));
	menuChangeMap_.emplace(MENU_TYPE::TIPS, std::bind(&TitleStateMain::ChangeMenuTips, this));
	menuChangeMap_.emplace(MENU_TYPE::EXIT, std::bind(&TitleStateMain::ChangeMenuExit, this));
}

TitleStateMain::~TitleStateMain()
{
	DeleteGraph(effectScreen_);
}

void TitleStateMain::Init()
{
	// ロゴの生成
	logo_ = std::make_unique<TitleLogo>();
	logo_->Init();

	// ボタン
	button_ = std::make_unique<TitleButton>();
	button_->Init();

	// オーブ
	orb_ = std::make_unique<TitleOrb>();
	orb_->Init();

	// ポストエフェクト
	ripples_ = std::make_unique<PostEffectRipples>(POST_EFFECT_PARAM);
	ripples_->Load();
	ripples_->Init();

	int screenOperationMessageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_OPERATION, FONT_THICK_OPERATION);
	screenOperationMessage_.fontHandle = screenOperationMessageFont;
	screenOperationMessage_.color = UtilityCommon::BLACK;
	screenOperationMessage_.pos = { 0, SCREEN_OPERATION_HEIGHT };
	screenOperationMessage_.string = GetOperationMessage();

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::TITLE_MAIN);

	// 初期透過率
	screenAlpha_ = UtilityCommon::ALPHA_MAX;

	// スクリーンの生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// メニュー項目別処理の登録
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, FONT_THICK);
	constexpr int FIRST_POS_Y = 400;
	constexpr int OFFSET = 60;
	for (int i = 0; i < MENU_MAX; i++)
	{
		menutextList_[i].fontHandle = font;
		menutextList_[i].string = textList_[i];
		menutextList_[i].color = UtilityCommon::BLACK;
		menutextList_[i].pos = { Application::SCREEN_HALF_X, FIRST_POS_Y + i * OFFSET };
	}

	// 初期選択位置の設定
	menuIndex_ = 0;
	menutextList_[menuIndex_].color = UtilityCommon::WHITE;

	// 選択ボックスの設定
	boxPos_ = { Application::SCREEN_HALF_X, menutextList_[menuIndex_].pos.y + FONT_SIZE / 2 };
	boxSize_ = { static_cast<int>(menutextList_[menuIndex_].string.length()) * FONT_SIZE + BOX_PADDING_X, FONT_SIZE + BOX_PADDING_Y };

	// 初期更新処理の登録
	ChangeState(STATE::WAIT);
	
	// 初期描画処理の登録
	draw_ = std::bind(&TitleStateMain::DrawWait, this);
}

void TitleStateMain::Update()
{
	// 一部状態別処理
	update_();

	// オーブの更新
	orb_->Update();
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

	// 状態別描画
	draw_();
	
	// オーブの描画
	orb_->Draw();
	
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
	// ロゴの更新
	logo_->Update();

	// ボタンの更新
	button_->Update();

	// ゲーム開始の入力をした場合
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		
		// フェードアウトへ遷移
		ChangeState(STATE::FADE_OUT);

		// フェード後は選択状態へ遷移
		nextState_ = STATE::SELECT;
	}
}

void TitleStateMain::UpdateEffect()
{
	step_ += scnMng_.GetDeltaTime();
	screenAlpha_ -= EFFECT_ALPHA_RATE;
	if (screenAlpha_ < 0.0f)
	{
		// 状態遷移
		parent_.ChangeState(SceneTitle::STATE::EXPLANATION);
	}
}

void TitleStateMain::UpdateSelect()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		// 増加
		menuIndex_ = UtilityCommon::WrapStepIndex(menuIndex_, 1, 0, MENU_MAX);

		// 選択テキストの色更新
		UpdateSelectTextColor();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_UP))
	{
		// 減少
		menuIndex_ = UtilityCommon::WrapStepIndex(menuIndex_, -1, 0, MENU_MAX);

		// 選択テキストの色更新
		UpdateSelectTextColor();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// メニュー項目の中身を実行
		menuChangeMap_[static_cast<MENU_TYPE>(menuIndex_)]();
		return;
	}
}

void TitleStateMain::UpdateFadeIn()
{
	// 透過率の加算
	screenAlpha_ += FADE_ALPHA_RATE;

	// 透過率が最大値を超えた場合
	if (screenAlpha_ > UtilityCommon::ALPHA_MAX)
	{
		// 透過率を固定
		screenAlpha_ = UtilityCommon::ALPHA_MAX;

		// 次回の状態へ遷移
		ChangeState(nextState_);
	}
}

void TitleStateMain::UpdateFadeOut()
{
	// 透過率の減少
	screenAlpha_ -= FADE_ALPHA_RATE;

	// 透過率が最小値を下回った場合
	if (screenAlpha_ < 0.0f)
	{
		// 透過率を固定
		screenAlpha_ = 0.0f;

		// 描画先の更新
		if (nextState_ == STATE::SELECT)
		{
			draw_ = std::bind(&TitleStateMain::DrawSelect, this);

			// 選択処理の初期化
			menuIndex_ = 0;
			UpdateSelectTextColor();
		}
		else
		{
			draw_ = std::bind(&TitleStateMain::DrawWait, this);
		}

		// フェードインへ遷移
		ChangeState(TitleStateMain::STATE::FADE_IN);
	}
}

void TitleStateMain::ChangeState(const STATE state)
{
	state_ = state;

	stateChangeMap_[state]();
}

void TitleStateMain::ChangeStateNone()
{
	update_ = std::bind(&TitleStateMain::UpdateNone, this);
}

void TitleStateMain::ChangeStateWait()
{
	update_ = std::bind(&TitleStateMain::UpdateWait, this);
}

void TitleStateMain::ChangeStateSelect()
{
	update_ = std::bind(&TitleStateMain::UpdateSelect, this);
}

void TitleStateMain::ChangeStateFadeIn()
{
	update_ = std::bind(&TitleStateMain::UpdateFadeIn, this);
}

void TitleStateMain::ChangeStateFadeOut()
{
	update_ = std::bind(&TitleStateMain::UpdateFadeOut, this);
}

void TitleStateMain::ChangeStateEffect()
{
	update_ = std::bind(&TitleStateMain::UpdateEffect, this);

	// エフェクトの再生
	ripples_->SetStart();

	// エフェクト個々の再生停止
	logo_->SetIsEffect(false);
	button_->SetIsEffect(false);
}

void TitleStateMain::ChangeMenuStart()
{
	// エフェクト開始
	ChangeState(STATE::EFFECT);

	// BGMの停止
	sndMng_.StopBgm(SoundType::BGM::TITLE_MAIN);

	// 効果音の再生
	sndMng_.PlaySe(SoundType::SE::GAME_START);
}

void TitleStateMain::ChangeMenuExplanation()
{
	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DECISION);

	// 説明画面へ遷移
	scnMng_.PushScene(std::make_unique<SceneExplanation>());
}

void TitleStateMain::ChangeMenuTips()
{
	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DECISION);

	// ヒント画面へ遷移
	scnMng_.PushScene(std::make_unique<SceneTips>());
}

void TitleStateMain::ChangeMenuSetting()
{
	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DECISION);
}

void TitleStateMain::ChangeMenuExit()
{
	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DECISION);

	// 前の画面へ戻る
	ChangeState(STATE::FADE_OUT);
	nextState_ = STATE::WAIT;
}

void TitleStateMain::DrawWait()
{
	// ボタン描画
	button_->Draw();
}

void TitleStateMain::DrawSelect()
{	
	DrawBox(
		boxPos_.x - boxSize_.x / 2,
		boxPos_.y - boxSize_.y / 2,
		boxPos_.x + boxSize_.x / 2,
		boxPos_.y + boxSize_.y / 2,
		UtilityCommon::BLACK,
		true
	);
	
	// 各種テキスト描画
	for (const auto& text : menutextList_)
	{
		text.DrawCenter();
	}

	// 画面の操作説明
	screenOperationMessage_.Draw();
}

void TitleStateMain::UpdateSelectTextColor()
{
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (i == menuIndex_)
		{
			menutextList_[i].color = UtilityCommon::WHITE;
		}
		else
		{
			menutextList_[i].color = UtilityCommon::BLACK;
		}
	}

	// ボックスの更新
	UpdateBox();
}

void TitleStateMain::UpdateBox()
{
	// 位置の高さのみ変更
	boxPos_.y = menutextList_[menuIndex_].pos.y + FONT_SIZE / 2;
	
	// サイズの幅のみ変更
	boxSize_.x = menutextList_[menuIndex_].string.length() * FONT_SIZE + BOX_PADDING_X;
}

const std::wstring TitleStateMain::GetOperationMessage() const
{
	if (GetJoypadNum() > 0)
	{
		return OPERATION_MESSAGE_PAD;
	}
	else
	{
		return OPERATION_MESSAGE_KEY;
	}
}