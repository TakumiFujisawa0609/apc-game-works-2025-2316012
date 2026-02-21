#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "../Core/Common/ControllerTextAnimation.h"
#include "../Core/Common/Timer.h"
#include "SceneResult.h"

SceneResult::SceneResult()
{
	isBgm_ = false;
	score_ = 0;
	wageIndex_ = 0;
	state_ = STATE::START;

	// 更新関数のセット
	updataFunc_ = std::bind(&SceneResult::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneResult::LoadingDraw, this);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{	
	// スコア管理クラスの取得
	ScoreManager& score = ScoreManager::GetInstance();

	// フォントの作成
	const std::wstring FONT_NAME = resMng_.GetFontName("fontKazuki");
	const int titleFont = fontMng_.CreateMyFont(FONT_NAME, TITLE_FONT_SIZE, 0);
	const int scoreFont = fontMng_.CreateMyFont(FONT_NAME, SCORE_FONT_SIZE, 0);
	const int commentFont = fontMng_.CreateMyFont(FONT_NAME, COMMENT_FONT_SIZE, 0);
	const int wageFont = fontMng_.CreateMyFont(FONT_NAME, WAGE_DETAILS_FONT_SIZE, 0);

	// タイトルテキスト
	titleText_.fontHandle = titleFont;
	titleText_.pos = Vector2{ TITLE_POS_X, TITLE_POS_Y };
	titleText_.color = UtilityCommon::BLACK;
	titleText_.string = L"本日のお賃金";

	// スコアテキスト
	scoreText_.fontHandle = scoreFont;
	scoreText_.pos = Vector2{ SCORE_POS_X, SCORE_POS_Y };
	scoreText_.color = UtilityCommon::BLACK;
	scoreText_.string = L"%d円";

	// ひとことテキスト
	commentText_.fontHandle = commentFont;
	commentText_.pos = Vector2{ COMMENT_POS_X, COMMENT_POS_Y };
	commentText_.color = UtilityCommon::BLACK;

	// 給与詳細テキスト
	for (int i = 0; i < WAGE_DETAILS_MAX; i++)
	{
		wageDetailsText_[i].fontHandle = wageFont;
		wageDetailsText_[i].string = L"";
		wageDetailsText_[i].color = UtilityCommon::BLACK;
		wageDetailsText_[i].pos = { WAGE_DETAILS_POS_X, WAGE_DETAILS_POS_Y + i * COMMENT_FONT_SIZE };
	}

	// 詳細別スコアの設定
	wageDetailsText_[static_cast<int>(WAGE_DETAILS::BASIC)].data1 = ScoreManager::DEFAULT_SCORE;
	wageDetailsText_[static_cast<int>(WAGE_DETAILS::BONUS)].data1 = score.GetBonusScore();
	wageDetailsText_[static_cast<int>(WAGE_DETAILS::CUT)].data1 = score.GetCutScore() * -1;

	// ランク
	rank_.handleIds = resMng_.GetHandles("ranks");
	rank_.index = 0;
	rank_.pos = { RANK_POS_X, RANK_POS_Y };

	// スコアの格納
	score_ = score.GetTotalScore();

	// タイマー
	timer_ = std::make_unique<Timer>(START_TIME);
	timer_->InitCountUp();

	// アニメーション
	textAnimation_ = std::make_unique<ControllerTextAnimation>(commentText_, TEXT_ANIMATION_TIME);
	textAnimation_->Init();

	isBgm_ = false;
	state_ = STATE::START;
	update_ = std::bind(&SceneResult::UpdateStart, this);

	// 開始音
	sndMng_.PlaySe(SoundType::SE::RESULT_START);
}

void SceneResult::NormalUpdate()
{
	update_();
}

void SceneResult::NormalDraw()
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

	// タイトル
	titleText_.DrawCenter();

	// ラインを描画
	DrawLine(
		LINE_POS_X_START,
		LINE_POS_Y,
		LINE_POS_X_GOAL,
		LINE_POS_Y,
		UtilityCommon::BLACK,
		LINE_THICKNESS
	);

	if (state_ != STATE::START)
	{
		// スコア
		scoreText_.DrawFormatCenter();
		
		// 給与詳細
		for(const auto & text : wageDetailsText_) 
		{
			text.DrawFormatCenter();
		}

		if (state_ == STATE::END)
		{
			// ランクの描画
			rank_.DrawRota();	

			// コメント
			textAnimation_->Draw();
		}
	}
}

void SceneResult::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneResult::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneResult::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn(Fader::STATE::IMMEDIATE);
}

void SceneResult::UpdateStart()
{
	if (timer_->CountUp())
	{
		// 状態遷移
		state_ = STATE::DRUM_ROLL;
		update_ = std::bind(&SceneResult::UpdateDrumRoll, this);

		// ドラムロール開始
		sndMng_.PlaySe(SoundType::SE::DRUM_ROLL);
	}
}

void SceneResult::UpdateDrumRoll()
{
	constexpr int ADD_VALUE = 60;
	scoreText_.data1 += ADD_VALUE;

	if (scoreText_.data1 > score_)
	{
		scoreText_.data1 = score_;
		sndMng_.StopSe(SoundType::SE::DRUM_ROLL);
		sndMng_.PlaySe(SoundType::SE::DRUM_ROLL_END);
		state_ = STATE::WAGE_DISPLAY;
		update_ = std::bind(&SceneResult::UpdateWage, this);
		timer_->SetGoalTime(WAGE_INTERVAL_TIME);
		timer_->InitCountUp();
	}
}

void SceneResult::UpdateWage()
{
	// 時間経過ごとにテキストの反映
	if (timer_->CountUp())
	{
		// インデックスが最大時
		if (wageIndex_ >= WAGE_DETAILS_MAX)
		{
			// 次の状態へ遷移			
			state_ = STATE::END;
			update_ = std::bind(&SceneResult::UpdateEnd, this);

			// BGMの再生
			sndMng_.PlayBgm(SoundType::BGM::RESULT);

			// ランク別にテキスト設定
			if (score_ >= RANK_SCORES[static_cast<int>(RANK::S)])
			{
				commentText_.string = RANK_COMMENTS[static_cast<int>(RANK::S)];
				rank_.index = static_cast<int>(RANK::S);
			}
			else if (score_ >= RANK_SCORES[static_cast<int>(RANK::A)])
			{
				commentText_.string = RANK_COMMENTS[static_cast<int>(RANK::A)];
				rank_.index = static_cast<int>(RANK::A);
			}
			else if (score_ >= RANK_SCORES[static_cast<int>(RANK::B)])
			{
				commentText_.string = RANK_COMMENTS[static_cast<int>(RANK::B)];
				rank_.index = static_cast<int>(RANK::B);
			}
			else if (score_ >= RANK_SCORES[static_cast<int>(RANK::C)])
			{
				commentText_.string = RANK_COMMENTS[static_cast<int>(RANK::C)];
				rank_.index = static_cast<int>(RANK::C);
			}
			else
			{
				commentText_.string = GAME_OVER_COMMENT;
				rank_.index = static_cast<int>(RANK::MAX);
			}

			// テキストアニメーション開始
			textAnimation_->SetCharacterString(commentText_);
			return;
		}

		// テキスト反映
		wageDetailsText_[wageIndex_].string = WAGE_DETAILS_TEXT[wageIndex_];

		// インデックス更新
		wageIndex_++;

		// タイマー初期化
		timer_->InitCountUp();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::WAGE_DISPLAY);
	}
}

void SceneResult::UpdateEnd()
{
	// テキストの更新
	if (!textAnimation_->IsEnd())
	{
		textAnimation_->Update();
	}

	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, Fader::STATE::FINISH);
		sndMng_.StopBgm(SoundType::BGM::RESULT);
	}

	//// 効果音の再生を終えた場合
	//if (!isBgm_ && !sndMng_.IsCheckPlaySe(SoundType::SE::DRUM_ROLL_END))
	//{
	//	sndMng_.PlayBgm(SoundType::BGM::RESULT);
	//	isBgm_ = true;
	//}
}