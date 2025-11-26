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
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneResult::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneResult::LoadingDraw, this);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Load()
{
	SceneBase::Load();
}

void SceneResult::Init()
{	
	SceneBase::Init();

	int titleFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 48, 0);
	int scoreFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 80, 0);
	int commentFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 32, 0);

	// タイトルテキスト
	titleText_.fontHandle = titleFont;
	titleText_.pos = Vector2{ Application::SCREEN_SIZE_X / 2, 60 };
	titleText_.color = UtilityCommon::BLACK;
	titleText_.string = L"結果発表";

	// スコアテキスト
	scoreText_.fontHandle = scoreFont;
	scoreText_.pos = Vector2{ Application::SCREEN_HALF_X,300 };
	scoreText_.color = UtilityCommon::BLACK;
	scoreText_.string = L"%d";

	// ひとことテキスト
	commentText_.fontHandle = commentFont;
	commentText_.pos = Vector2{ Application::SCREEN_HALF_X, 500 };
	commentText_.color = UtilityCommon::BLACK;

	// ランク
	rank_.handleIds = resMng_.GetHandles("ranks");
	rank_.index = 0;
	rank_.pos = { 1000, 500 };

	// スコアの格納
	ScoreManager& score = ScoreManager::GetInstance();
	score_ = score.GetScore();

	// タイマー
	timer_ = std::make_unique<Timer>(1.0f);
	timer_->InitCountUp();

	// アニメーション
	textAnimation_ = std::make_unique<ControllerTextAnimation>(commentText_, 0.08f);
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
		if (score_ >= RANK_S)
		{
			commentText_.string = L"大変よくできました";
			rank_.index = 0;
		}
		else if (score_ >= RANK_A)
		{
			commentText_.string = L"素晴らしい結果です";
			rank_.index = 1;
		}
		else if (score_ >= RANK_B)
		{
			commentText_.string = L"良い結果です";
			rank_.index = 2;
		}
		else if (score_ >= RANK_C)
		{
			commentText_.string = L"もっと頑張りましょう";
			rank_.index = 3;
		}
		else
		{
			commentText_.string = L"死んでしまうとは情けないですね";
			rank_.index = 4;
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