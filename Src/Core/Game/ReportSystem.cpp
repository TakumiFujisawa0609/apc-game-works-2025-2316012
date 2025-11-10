#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/GameStateManager.h"
#include "../../Manager/Generic/CharacterManager.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Manager/Resource/FontManager.h"
#include "../../Manager/Resource/SoundManager.h"
#include "../../Manager/System/GameSystemManager.h"
#include "../../Manager/System/ScoreManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Object/Actor/Character/Player.h"
#include "../Common/Timer.h"
#include "../Common/ControllerTextAnimation.h"
#include "Message.h"
#include "ReportSystem.h"

ReportSystem::ReportSystem(Player& player) :
	player_(player),
	scoreMng_(ScoreManager::GetInstance())
{	
	// 各種変数の初期化
	state_ = STATE::NONE;
	commaStep_ = 0.0f;
	reportPercent_ = 0.0f;

	// 処理の登録
	RegisterStateFunction(STATE::WAIT, std::bind(&ReportSystem::UpdateWait, this), std::bind(&ReportSystem::DrawWait, this));
	RegisterStateFunction(STATE::REPORTING, std::bind(&ReportSystem::UpdateReporting, this), std::bind(&ReportSystem::DrawReporting, this));
}

ReportSystem::~ReportSystem()
{
}

void ReportSystem::Load()
{
	int font = FontManager::GetInstance().CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, FONT_THICK);
	const Vector2 POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// テキストの設定
	reportingText_.fontHandle = font;
	reportingText_.pos = POS;
	reportingText_.color = UtilityCommon::WHITE;
	reportingText_.string = REPORTING_TEXT;

	// ゲージの設定
	gauge_.handle = resMng_.GetHandle("reportGauge");

	// タイマーの生成
	timer_ = std::make_unique<Timer>(REPORTING_TIME);
}

void ReportSystem::Init()
{
	// 初期状態
	state_ = STATE::WAIT;

	// タイマーの初期化
	timer_->InitCountUp();

	// ゲージの初期設定
	gauge_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y }; //中心に描画
}

void ReportSystem::Update()
{
	stateUpdateMap_[state_]();
}

void ReportSystem::Draw()
{
	stateDrawMap_[state_]();
}

void ReportSystem::ChangeReporting()
{
	// 状態変更
	state_ = STATE::REPORTING;

	// タイマー時間の設定
	timer_->SetGoalTime(REPORTING_TIME);
}

void ReportSystem::RegisterStateFunction(const STATE state, std::function<void()> update, std::function<void()> draw)
{
	stateUpdateMap_[state] = update;
	stateDrawMap_[state] = draw;
}

void ReportSystem::UpdateWait()
{
	// 進捗率を反映
	gauge_.percent = player_.GetReportPercent();

	// 最大まで達したとき
	if (gauge_.percent > GAUGE_MAX)
	{
		// ゲージ初期化
		gauge_.percent = 0.0f;

		// プレイヤーの進捗率も初期化
		player_.SetReportPercent(0.0f);

		// テキストの表示時間を設定
		timer_->SetGoalTime(TEXT_DISPLAY_TIME);

		// テキストをミスに設定
		systemMng_.ChangeMessage(Message::TYPE::REPORT_MISS);

		// ミスしたとき用にスコアを減算
		scoreMng_.AddScore(SCORE_MISS);
	}
}

void ReportSystem::UpdateReporting()
{
	// 時間に達した場合
	if (timer_->CountUp())
	{
		// ゲーム状態をプレイにする
		stateMng_.ChangeState(GameStateManager::STATE::PLAY);

		// 状態変更
		state_ = STATE::WAIT;

		// テキストを完了に設定
		systemMng_.ChangeMessage(Message::TYPE::REPORT_COMPLITE);

		// 文字列の初期化
		reportingText_.string = REPORTING_TEXT;
		commaStep_ = 0.0f;

		// 効果音の停止
		sndMng_.StopSe(SoundType::SE::REPORTING);

		// 成功したときのスコア加算
		scoreMng_.AddScore(SCORE_SUCCESS);
		return;
	}

	// コンマの追加
	constexpr float COMMA_TIME = 0.75f;
	constexpr int COMMA_MAX_NUM = 4;

	commaStep_ += scnMng_.GetDeltaTime();
	int count = static_cast<int>(commaStep_ / COMMA_TIME);
	count %= COMMA_MAX_NUM;
	reportingText_.string = REPORTING_TEXT;
	std::wstring dotStr = L".";

	for (int i = 0; i < count; i++)
	{
		reportingText_.string += dotStr;
	}
}

void ReportSystem::DrawWait()
{	
	// ゲージ量が0より大きい場合
	if (gauge_.percent > 0.0f)
	{
		// 描画
		gauge_.Draw();
	}
}

void ReportSystem::DrawReporting()
{
	// 黒背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::BLACK, true);

	// 文字列の描画
	reportingText_.DrawCenter();
}