#include <vector>
#include "../../../Application.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/InputManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/FontManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Core/PostEffect/PostEffectTitleSelect.h"
#include "../../../Core/Common/GlitchScreen.h"
#include "TitleStateSelect.h"

TitleStateSelect::TitleStateSelect(SceneTitle& parent) :
	TitleStateBase(parent)
{
	// 初期時の更新処理と描画処理
	update_ = std::bind(&TitleStateSelect::UpdateSelect, this);
	draw_ = std::bind(&TitleStateSelect::DrawSelect, this);

	// 状態遷移の登録
	changeMap_.emplace(TYPE::START, std::bind(&TitleStateSelect::ChangeStart, this));
	changeMap_.emplace(TYPE::END, std::bind(&TitleStateSelect::ChangeEnd, this));
	effectScreen_ = -1;
	afterStep_ = 0.0f;
	type_ = -1;
	effect_ = nullptr;
	glitch_ = nullptr;
}

TitleStateSelect::~TitleStateSelect()
{
	DeleteGraph(effectScreen_);
}

void TitleStateSelect::Init()
{
	// フォント
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 52, 0);

	// テキストの設定
	selectTexts_[0].pos = { Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y / 3 * 1 };
	selectTexts_[0].color = UtilityCommon::WHITE;
	selectTexts_[0].fontHandle = font;
	selectTexts_[0].string = L"開始する";

	selectTexts_[1].pos = { Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y / 3 * 2 };
	selectTexts_[1].color = UtilityCommon::WHITE;
	selectTexts_[1].fontHandle = font;
	selectTexts_[1].string = L"終了する";

	selectBack_.handleId = resMng_.GetHandle("selectBack");
	selectBack_.pos = selectTexts_[0].pos;
	selectBack_.size.x = selectTexts_[0].string.length() * 52;
	selectBack_.size.y = 60;

	// 選択後のテキスト設定
	afterText_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	afterText_.color = UtilityCommon::BLACK;
	afterText_.fontHandle = font;

	// グリッチ生成
	glitch_ = std::make_unique<GlitchScreen>();
	glitch_->Init();

	// エフェクトの作成
	effect_ = std::make_unique<PostEffectTitleSelect>();
	effect_->Init();

	// スクリーンの生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 初期選択項目
	type_ = 0;
}

void TitleStateSelect::Update()
{
	// 更新処理
	update_();
}

void TitleStateSelect::Draw()
{
	// 背景
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::RED,
		true
	);

	// 描画処理
	draw_();

	// グリッチの描画
	glitch_->Draw();

	////スクリーンの設定
	//SetDrawScreen(effectScreen_);

	//// 画面を初期化
	//ClearDrawScreen();

	//// ポストエフェクト描画
	//effect_->Draw();

	//// メインに戻す
	//SetDrawScreen(scnMng_.GetMainScreen());

	//// 描画
	//DrawGraph(0, 0, effectScreen_, false);

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleStateSelect::ChangeStart()
{
	// 処理の切り替え
	update_ = std::bind(&TitleStateSelect::UpdateStart, this);
	draw_ = std::bind(&TitleStateSelect::DrawAfter, this);

	// テキストの設定
	afterText_.string = L"仕事を開始します";
}

void TitleStateSelect::ChangeEnd()
{
	// 処理の切り替え
	update_ = std::bind(&TitleStateSelect::UpdateEnd, this);
	draw_ = std::bind(&TitleStateSelect::DrawAfter, this);

	// テキストの設定
	afterText_.string = L"システムを終了します";
}

void TitleStateSelect::UpdateSelect()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_UP))
	{
		// 増加
		type_ = UtilityCommon::WrapStepIndex(type_, 1, 0, TYPE_MAX);
		sndMng_.PlaySe(SoundType::SE::SELECT);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		// 減少
		type_ = UtilityCommon::WrapStepIndex(type_, -1, 0, TYPE_MAX);
		sndMng_.PlaySe(SoundType::SE::SELECT);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 状態遷移
		changeMap_[static_cast<TYPE>(type_)]();
		sndMng_.PlaySe(SoundType::SE::DECISION);
	}

	selectBack_.pos = selectTexts_[type_].pos;
	selectBack_.size.x = selectTexts_[type_].string.length() * 52;
	selectBack_.size.y = 60;
}

void TitleStateSelect::UpdateStart()
{
	afterStep_ += scnMng_.GetDeltaTime();

	if (afterStep_ > 1.0f)
	{
		// シーン遷移
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME, Fader::STATE::FINISH);

		// 更新処理変更
		update_ = std::bind(&TitleStateSelect::UpdateNone, this);
	}
}

void TitleStateSelect::UpdateEnd()
{
	afterStep_ += scnMng_.GetDeltaTime();

	if (afterStep_ > 1.0f)
	{
		// 終了処理
		Application::GetInstance().GameEnd();

		// 更新処理変更
		update_ = std::bind(&TitleStateSelect::UpdateNone, this);

		// SE停止
		sndMng_.StopAllSe();

		// BGM停止
		sndMng_.PlayBgm(SoundType::BGM::TITLE_SELECT);
	}
}

void TitleStateSelect::DrawSelect()
{
	selectBack_.DrawRota();
	for (int i = 0; i < TYPE_MAX; i++)
	{

		// 描画
		selectTexts_[i].DrawCenter();
	}
}

void TitleStateSelect::DrawAfter()
{
	afterText_.DrawCenter();
}