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

// スコアマネージャーの終了状態の省略形
using END = ScoreManager::END_STATE;

SceneResult::SceneResult()
{
	isBgm_ = false;
	score_ = 0;
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
	// フォントの作成
	const std::wstring FONT_NAME = resMng_.GetFontName("fontKazuki");
	const int titleFont = fontMng_.CreateMyFont(FONT_NAME, TITLE_FONT_SIZE, 0);
	const int scoreFont = fontMng_.CreateMyFont(FONT_NAME, SCORE_FONT_SIZE, 0);
	const int commentFont = fontMng_.CreateMyFont(FONT_NAME, COMMENT_FONT_SIZE, 0);

	// タイトルテキスト
	titleText_.fontHandle = titleFont;
	titleText_.pos = Vector2{ TITLE_POS_X, TITLE_POS_Y };
	titleText_.color = UtilityCommon::BLACK;
	titleText_.string = L"結果発表";

	// スコアテキスト
	scoreText_.fontHandle = scoreFont;
	scoreText_.pos = Vector2{ SCORE_POS_X, SCORE_POS_Y };
	scoreText_.color = UtilityCommon::BLACK;
	scoreText_.string = L"%d";

	// ひとことテキスト
	commentText_.fontHandle = commentFont;
	commentText_.pos = Vector2{ COMMENT_POS_X, COMMENT_POS_Y };
	commentText_.color = UtilityCommon::BLACK;

	// ランク
	rank_.handleIds = resMng_.GetHandles("ranks");
	rank_.index = 0;
	rank_.pos = { RANK_POS_X, RANK_POS_Y };

	// スコアの格納
	ScoreManager& score = ScoreManager::GetInstance();
	score_ = score.GetScore();

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

	if (state_ != STATE::START)
	{
		// スコア
		scoreText_.DrawFormatCenter();

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
		state_ = STATE::END;
		update_ = std::bind(&SceneResult::UpdateEnd, this);

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

		textAnimation_->SetCharacterString(commentText_);
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

	// 効果音の再生を終えた場合
	if (!isBgm_ && !sndMng_.IsCheckPlaySe(SoundType::SE::DRUM_ROLL_END))
	{
		sndMng_.PlayBgm(SoundType::BGM::RESULT);
		isBgm_ = true;
	}
}