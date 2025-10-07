#include "../../../Application.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Resource/FontManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Common/Timer.h"
#include "../../Common/ControllerTextAnimation.h"
#include "ReportSystem.h"

ReportSystem::ReportSystem() :
	input_(InputManager::GetInstance())
{
	// 各種変数の初期化
	state_ = STATE::NONE;
	commaStep_ = 0.0f;

	// 処理の登録
	RegisterStateFunction(STATE::WAIT, std::bind(&ReportSystem::UpdateWait, this), std::bind(&ReportSystem::DrawWait, this));
	RegisterStateFunction(STATE::REPORT, std::bind(&ReportSystem::UpdateReport, this), std::bind(&ReportSystem::DrawReport, this));
	RegisterStateFunction(STATE::COMPLETE, std::bind(&ReportSystem::UpdtateComplete, this), std::bind(&ReportSystem::DrawComplite, this));
	RegisterStateFunction(STATE::MISS, std::bind(&ReportSystem::UpdateMiss, this), std::bind(&ReportSystem::DrawMiss, this));
	RegisterStateFunction(STATE::REPORTING, std::bind(&ReportSystem::UpdateReporting, this), std::bind(&ReportSystem::DrawReporting, this));
}

ReportSystem::~ReportSystem()
{
}

void ReportSystem::Load()
{
	int font = FontManager::GetInstance().CreateMyFont(resMng_.GetFontName("fontDot"), FONT_SIZE, FONT_THICK);
	const Vector2 POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// テキストの設定
	reportingText_.fontHandle = font;
	reportingText_.pos = POS;
	reportingText_.color = UtilityCommon::WHITE;
	reportingText_.string = REPORTING_TEXT;

	compliteText_.fontHandle = font;
	compliteText_.pos = POS;
	compliteText_.color = UtilityCommon::WHITE;
	compliteText_.string = COMPLITE_TEXT;

	missText_.fontHandle = font;
	missText_.pos = POS;
	missText_.color = UtilityCommon::WHITE;
	missText_.string = MISS_TEXT;

	// ゲージの設定
	gauge_.handle = resMng_.GetHandle("reportGauge");

	// タイマーの生成
	timer_ = std::make_unique<Timer>(REPORTING_TIME);

	// アニメーションの初期化
	textAnimation_ = std::make_unique<ControllerTextAnimation>(compliteText_, 0.2f);
	textAnimation_->Init();
}

void ReportSystem::Init()
{
	// 初期状態
	state_ = STATE::WAIT;

	// タイマーの初期化
	timer_->Init();

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

void ReportSystem::RegisterStateFunction(const STATE state, std::function<void()> update, std::function<void()> draw)
{
	stateUpdateMap_[state] = update;
	stateDrawMap_[state] = draw;
}

void ReportSystem::UpdateWait()
{
	// 入力された場合
	if (input_.IsTrgDown(InputManager::TYPE::ANOMARY_REPORT))
	{
		// 状態変更
		state_ = STATE::REPORT;

		// ゲージ初期化
		gauge_.percent = 0.0f;
	}
}

void ReportSystem::UpdateReport()
{
	// 入力離した場合
	if (input_.IsTrgUp(InputManager::TYPE::ANOMARY_REPORT))
	{
		// 状態を戻す
		state_ = STATE::WAIT;
	}

	constexpr float SECOND = 3.0f;			// 時間
	constexpr float GAUGE_MAX = 100.0f;		// ゲージの最大値

	// ゲージの更新値を計算
	float step = GAUGE_MAX / SECOND * scnMng_.GetDeltaTime();

	// ゲージの更新
	gauge_.percent += step;

	// ゲージの値が最大に達したとき
	if (gauge_.percent >= GAUGE_MAX)
	{
		// 状態変更
		state_ = STATE::REPORTING;
	}
}

void ReportSystem::UpdateReporting()
{
	// 時間に達した場合
	if (timer_->CountDown())
	{
		// 状態変更
		state_ = STATE::COMPLETE;

		// タイマー初期化
		timer_->Init();

		// タイマーの目標時間をComplite用に変更
		timer_->SetGoalTime(COMPLITE_TEXT_DISPLAY_TIME);

		// アニメーション文字列の対象を変更
		textAnimation_->SetCharacterString(compliteText_);

		// 文字列の初期化
		reportingText_.string = REPORTING_TEXT;
		commaStep_ = 0.0f;
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

void ReportSystem::UpdateMiss()
{
	// テキストの表示
}

void ReportSystem::UpdtateComplete()
{
	// アニメーションを終えている場合
	if(textAnimation_->IsEnd())
	{
		// まだ数秒表示させるためタイマーの更新
		if (timer_->CountDown())
		{
			// 状態変更
			state_ = STATE::WAIT;
		}

		return;
	}

	// テキストアニメーションの更新
	textAnimation_->Update();

	// 更新を受け付ける
	UpdateWait();
}

void ReportSystem::DrawWait()
{
}

void ReportSystem::DrawReport()
{
	gauge_.Draw();
}

void ReportSystem::DrawReporting()
{
	// 黒背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::BLACK, true);

	// 文字列の描画
	reportingText_.DrawCenter();
}

void ReportSystem::DrawMiss()
{
	textAnimation_->Draw();
}

void ReportSystem::DrawComplite()
{
	textAnimation_->Draw();
}

void ReportSystem::CreateLineCollider()
{
	
}
