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
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 16, 0);

	int mainFont = FontManager::GetInstance().CreateMyFont(resMng_.GetFontName("fontKazuki"), 24, 0);
	const Vector2 POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 24 / 2 };

	imgWindow_ = resMng_.GetHandle("window");

	// テキストの設定
	text_.string = texts_.front();
	text_.fontHandle = mainFont;
	text_.pos = POS;
	text_.color = UtilityCommon::WHITE;

	skipText_.color = UtilityCommon::BLACK;
	skipText_.pos = { Application::SCREEN_SIZE_X - 120, Application::SCREEN_SIZE_Y - 20 };
	skipText_.fontHandle = font;
	skipText_.string = L"Enterでスキップ";

	subSprite_.handleIds = resMng_.GetHandles("titleExplanation");
	subSprite_.pos = { START_POS_X, START_POS_Y };
	subSprite_.index = 0;

	mainSprite_.pos = { Application::SCREEN_HALF_X, subSprite_.pos.y };

	timer_ = std::make_unique<Timer>(1.5f);

	textAnimationController_ = std::make_unique<ControllerTextAnimation>(text_, 0.12f);

	// テキストのアニメーション初期化
	textAnimationController_->Init();

	// タイマーの初期化
	timer_->InitCountUp();

	// 背景の設定
	backBoxPos_ = { 0, Application::SCREEN_HALF_Y - 30 };
	backBoxSize_ = { Application::SCREEN_SIZE_X, 60 };

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
		timer_->SetGoalTime(2.0f);
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
				timer_->SetGoalTime(0.08f);
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
	//アニメーション時間
	constexpr float ANIM_TIME = 1.5f;

	// 移動速度
	constexpr float MOVE_SPEED = 5.0f;

	// メインの終了位置
	constexpr float END_POS_X = -128;

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
	if (timer_->CountUp())
	{
		// 座標ランダム生成
		Vector2 pos = {};
		pos.x = 128 + GetRand(Application::SCREEN_SIZE_X - 128);
		pos.y = 128 + GetRand(Application::SCREEN_SIZE_Y - 128);

		float scales[5] = { 0.7f, 0.9f, 1.0f, 1.2f, 1.3f };

		// 画像生成
		Image img = {};
		img.handleId = imgWindow_;
		img.pos = pos;
		img.scale = scales[GetRand(4)];
		windows_.push_back(img);

		// クールタイム設定
		timer_->InitCountUp();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::CAVEAT);
	}

	if (windows_.size() > 50)
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(backBoxPos_.x, backBoxPos_.y, backBoxPos_.x + backBoxSize_.x, backBoxPos_.y + backBoxSize_.y, UtilityCommon::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	textAnimationController_->Draw();

	if (mainSprite_.handleIds != nullptr) { mainSprite_.DrawRota(); }

	if (subSprite_.handleIds != nullptr) { subSprite_.DrawRota(); }

	//DrawCircle(mainSprite_.pos.x, mainSprite_.pos.y, 30.0f, UtilityCommon::RED, true);
	//DrawCircle(subSprite_.pos.x, subSprite_.pos.y, 30.0f, UtilityCommon::YELLOW, true);

	//DrawFormatString(0, 0, 0xff0000, L"メインの座標%d,%d", mainSprite_.pos.x, mainSprite_.pos.y);
	//DrawFormatString(0, 20, 0xff0000, L"サブの座標%d,%d", subSprite_.pos.x, subSprite_.pos.y);
	//DrawFormatString(0, 40, 0xff0000, L"移動ステップ%2f", moveStep_);
	//DrawFormatString(0, 60, 0xff0000, isUpdateSprite_ ? L"true" : L"false");
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
	std::vector<int> skipIndexs = { 0, 1, 3, 5, 13 };

	for (int index : skipIndexs)
	{
		if (textIndex_ == index)
		{
			return true;
		}
	}

	return false;
}
