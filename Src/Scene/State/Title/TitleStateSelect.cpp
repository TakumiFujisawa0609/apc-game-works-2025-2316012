#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../../Manager/Resource/FontManager.h"
#include "../../../Utility/UtilityCommon.h"
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

	afterStep_ = 0.0f;
	type_ = -1;
}

TitleStateSelect::~TitleStateSelect()
{
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

	// 選択後のテキスト設定
	afterText_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	afterText_.color = UtilityCommon::BLACK;
	afterText_.fontHandle = font;

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
	update_ = std::bind(&TitleStateSelect::UpdateNone, this);
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
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		// 減少
		type_ = UtilityCommon::WrapStepIndex(type_, -1, 0, TYPE_MAX);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 状態遷移
		changeMap_[static_cast<TYPE>(type_)]();
	}
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
	}
}

void TitleStateSelect::DrawSelect()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		// 色を変える
		selectTexts_[i].color = type_ == i ? UtilityCommon::BLACK : UtilityCommon::WHITE;

		// 描画
		selectTexts_[i].DrawCenter();
	}
}

void TitleStateSelect::DrawAfter()
{
	afterText_.DrawCenter();
}