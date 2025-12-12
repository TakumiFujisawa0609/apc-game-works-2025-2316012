#include "../../../Application.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/InputManager.h"
#include "../../../Manager/Common/FontManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Core/Common/ControllerTextAnimation.h"
#include "../../../Core/Common/Timer.h"
#include "../../SceneTitle.h"
#include "TitleStateExplanation.h"

namespace
{
	// 開始位置
	constexpr int START_POS_X = Application::SCREEN_SIZE_X + 128;
	constexpr int START_POS_Y = Application::SCREEN_SIZE_Y - 150;
}

TitleStateExplanation::TitleStateExplanation(SceneTitle& parent) :
	TitleStateBase(parent)
{
	RegisterStateFunctions(STATE::WAIT, std::bind(&TitleStateExplanation::UpdateWait, this), std::bind(&TitleStateExplanation::DrawNone, this));
	RegisterStateFunctions(STATE::TEXT_DISPLAY, std::bind(&TitleStateExplanation::UpdateTextDisplay, this), std::bind(&TitleStateExplanation::DrawTextDisplay, this));
	RegisterStateFunctions(STATE::BUG, std::bind(&TitleStateExplanation::UpdateBug, this), std::bind(&TitleStateExplanation::DrawBug, this));

	textIndex_ = 0;
	moveStep_ = 0.0f;
}

TitleStateExplanation::~TitleStateExplanation()
{
}

void TitleStateExplanation::Init()
{
	// フォント生成
	const std::wstring fontName = resMng_.GetFontName("fontKazuki");
	const int skipFont = fontMng_.CreateMyFont(fontName, SKIP_FONT_SIZE, 0);
	const int mainFont = FontManager::GetInstance().CreateMyFont(fontName, MAIN_FONT_SIZE, 0);

	// テキスト位置
	const Vector2 POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - MAIN_FONT_SIZE / 2 };

	// 表示画像の設定
	imgWindow_ = resMng_.GetHandle("window");	
	
	subSprite_.handleIds = resMng_.GetHandles("titleExplanation");
	subSprite_.pos = { START_POS_X, START_POS_Y };
	subSprite_.index = 0;
	
	mainSprite_.pos = { Application::SCREEN_HALF_X, subSprite_.pos.y };

	// テキストの設定
	text_.string = texts_.front();
	text_.fontHandle = mainFont;
	text_.pos = POS;
	text_.color = UtilityCommon::WHITE;

	skipText_.color = UtilityCommon::BLACK;
	skipText_.pos = { SKIP_TEXT_POS_X, SKIP_TEXT_POS_Y };
	skipText_.fontHandle = skipFont;
	skipText_.string = SKIP_TEXT;

	// タイマー設定
	timer_ = std::make_unique<Timer>(WAIT_TIME);

	// テキストアニメーションコントローラー生成
	textAnimationController_ = std::make_unique<ControllerTextAnimation>(text_, TEXT_ANIMATION_SPEED);

	// テキストのアニメーション初期化
	textAnimationController_->Init();

	// タイマーの初期化
	timer_->InitCountUp();

	// 背景の設定
	backBoxPos_ = { BACK_BOX_POS_X,BACK_BOX_POS_Y };
	backBoxSize_ = { BACK_BOX_SIZE_X, BACK_BOX_SIZE_Y };

	// 状態設定
	state_ = STATE::WAIT;

	isUpdateSprite_ = IsSkipSpriteChange();
}

void TitleStateExplanation::Update()
{
	// スキップの入力がされた場合
	if (inputMng_.IsTrgDown(InputManager::TYPE::EXPLANTION_SKIP))
	{
		// シーン遷移
		parent_.ChangeState(SceneTitle::STATE::SELECT);
		// BGM停止
		sndMng_.StopBgm(SoundType::BGM::TITLE_EXPLANATION);
		sndMng_.PlayBgm(SoundType::BGM::TITLE_SELECT);
		return;
	}

	stateUpdateMap_[state_]();
}

void TitleStateExplanation::Draw()
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

	stateDrawMap_[state_]();

	// テキストの描画
	skipText_.Draw();
}

void TitleStateExplanation::RegisterStateFunctions(const STATE state, std::function<void()> update, std::function<void()> draw)
{
	stateUpdateMap_[state] = update;
	stateDrawMap_[state] = draw;
}

void TitleStateExplanation::UpdateWait()
{
	if (timer_->CountUp())
	{
		state_ = STATE::TEXT_DISPLAY;
		timer_->SetGoalTime(TEXT_DISPLAY_TIME);
		timer_->InitCountUp();
		sndMng_.PlayBgm(SoundType::BGM::TITLE_EXPLANATION);
		return;
	}
}

void TitleStateExplanation::UpdateTextDisplay()
{
	// アニメーションを終えている場合
	if (textAnimationController_->IsEnd())
	{
		// まだ数秒表示させるためタイマーの更新
		if (timer_->CountUp())
		{
			// テキストのカウントを更新
			textIndex_++;

			// テキストがまだ残っている場合
			if (textIndex_ < static_cast<int>(texts_.size()))
			{
				// 次のテキストに変更
				text_.string = texts_[textIndex_];

				// テキストアニメーションの再設定
				textAnimationController_->SetCharacterString(text_);

				// タイマーの初期化
				timer_->InitCountUp();

				// スプライトの更新
				isUpdateSprite_ = IsSkipSpriteChange();
			}
			else
			{
				// 状態変更
				state_ = STATE::BUG;

				// BGM停止
				sndMng_.StopBgm(SoundType::BGM::TITLE_EXPLANATION);

				// 効果音の再生
				sndMng_.PlaySe(SoundType::SE::BUG_1);
				sndMng_.PlaySe(SoundType::SE::BUG_2);

				// 画像生成クールタイム設定
				timer_->SetGoalTime(WINDOW_CREATE_COOL_TIME);
			}
		}

		return;
	}

	// テキストアニメーションの更新
	textAnimationController_->Update();

	UpdateSprite();
}

void TitleStateExplanation::UpdateSprite()
{

	if (isUpdateSprite_)
	{
		return;
	}

	//時間更新
	moveStep_ += scnMng_.GetDeltaTime();

	//座標計算
	subSprite_.pos.x = static_cast<int>(UtilityCommon::EaseOutQuad(moveStep_, ANIM_TIME, static_cast<float>(START_POS_X), static_cast<float>(Application::SCREEN_HALF_X)));
	mainSprite_.pos.x = static_cast<int>(UtilityCommon::EaseOutQuad(moveStep_, ANIM_TIME, static_cast<float>(Application::SCREEN_HALF_X), static_cast<float>(END_POS_X)));

	if (subSprite_.pos.x <= Application::SCREEN_HALF_X)
	{
		isUpdateSprite_ = true;
		
		// 上書き
		mainSprite_ = subSprite_;

		subSprite_.pos = { START_POS_X, START_POS_Y };
		int newTextureIndex = mainSprite_.index + 1;
		if (newTextureIndex == 8) { mainSprite_.index = 7; }
		mainSprite_.pos = { Application::SCREEN_HALF_X, subSprite_.pos.y };
		subSprite_.index = newTextureIndex;
		moveStep_ = 0.0f;
	}
}

void TitleStateExplanation::UpdateBug()
{
	constexpr int CREATE_COUNT = 50;
	constexpr int OFFSET_POS = 128;

	if (timer_->CountUp())
	{
		// 座標ランダム生成
		Vector2 pos = {};
		pos.x = OFFSET_POS + GetRand(Application::SCREEN_SIZE_X - OFFSET_POS);
		pos.y = OFFSET_POS + GetRand(Application::SCREEN_SIZE_Y - OFFSET_POS);

		// 画像生成
		Image img = {};
		img.handleId = imgWindow_;
		img.pos = pos;
		img.scale = IMG_SCALES[GetRand(static_cast<int>(IMG_SCALES.size()) - 1)];
		windows_.push_back(img);

		// クールタイム設定
		timer_->InitCountUp();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::CAVEAT);
	}

	if (windows_.size() > CREATE_COUNT)
	{
		parent_.ChangeState(SceneTitle::STATE::SELECT);
		sndMng_.StopAllSe();
		sndMng_.PlayBgm(SoundType::BGM::TITLE_SELECT);
		return;
	}
}

void TitleStateExplanation::DrawTextDisplay()
{
	// 背景の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	DrawBox(backBoxPos_.x, backBoxPos_.y, backBoxPos_.x + backBoxSize_.x, backBoxPos_.y + backBoxSize_.y, UtilityCommon::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキストの描画
	textAnimationController_->Draw();

	// スプライトの描画
	if (mainSprite_.handleIds != nullptr) { mainSprite_.DrawRota(); }
	if (subSprite_.handleIds != nullptr) { subSprite_.DrawRota(); }
}

void TitleStateExplanation::DrawBug()
{
	DrawTextDisplay();

	for (auto& w : windows_)
	{
		w.DrawRota();
	}
}

bool TitleStateExplanation::IsSkipSpriteChange()
{
	for (int index : SKIP_INDEXS)
	{
		if (textIndex_ == index)
		{
			return true;
		}
	}
	return false;
}
