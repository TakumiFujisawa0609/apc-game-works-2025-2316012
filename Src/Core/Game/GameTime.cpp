#include <chrono>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Game/GameSystemManager.h"
#include "../../Manager/Common/ScoreManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "Message.h"
#include "GameTime.h"

GameTime::GameTime(const Json& param) :
	FONT_NAME(param["fontName"]),
	FONT_SIZE(param["fontSize"]),
	FONT_THICK(param["fontThick"]),
	GAME_TIME(param["gameTime"]),
	DATE_POS{ param["datePos"]["x"], param["datePos"]["y"] },
	TIME_POS{ param["timePos"]["x"], param["timePos"]["y"] },
	stateMng_(GameStateManager::GetInstance())
{
	isEvent_ = false;
	todayText_ = CharacterString();
	text_ = CharacterString();
}

GameTime::~GameTime()
{
}

void GameTime::Load()
{
	// 時間
	timer_ = std::make_unique<Timer>(GAME_TIME);

	// フォント
	int font = fontMng_.CreateMyFont(resMng_.GetFontName(FONT_NAME), FONT_SIZE, FONT_THICK);

	// 日付文字列の設定
	todayText_.pos = DATE_POS;
	todayText_.color = UtilityCommon::WHITE;
	todayText_.string = GetYmdWstring();

	// 時間文字列の設定
	text_.pos = TIME_POS;
	text_.color = UtilityCommon::WHITE;
	text_.string = L"%d時%d分";

	// フォントの反映
	todayText_.fontHandle = font;
	text_.fontHandle = font;
}

void GameTime::Init()
{
	// タイマー初期化
	timer_->InitCountUp();

	// イベント設定
	isEvent_ = false;
}

void GameTime::Update()
{
	// 制限時間に達した場合
	if (timer_->CountUp())
	{
		// ゲームの終了処理
		stateMng_.SetGameClear();
		return;
	}

	if (timer_->GetCount() >= ONE_MINUTES && !isEvent_)
	{
		systemMng_.ChangeMessage(Message::TYPE::ONE_MINNUTES_LATER);
		isEvent_ = true;
	}

}

void GameTime::Draw()
{
	// 日付の描画
	todayText_.Draw();

	// 時間の計算
	int count = timer_->GetCount();
	int hour = count / ONE_MINUTES;
	int minute = count % ONE_MINUTES;

	// テキストの情報反映
	text_.data1 = hour;
	text_.data2 = minute;

	// テキストの描画
	text_.Draw();
}

std::wstring GameTime::GetYmdWstring()
{
	// 現在の日付と時刻を取得
	auto now = std::chrono::system_clock::now();

	// 日付の取得
	auto today = std::chrono::floor<std::chrono::days>(now);

	// 年、月、日の構造体に分解
	auto ymd = std::chrono::year_month_day(today);

	// 情報をもとに日付のストリングを生成
	std::wstring ymdString =
		std::to_wstring((int)ymd.year()) + L"年" +
		std::to_wstring((unsigned)ymd.month()) + L"月" +
		std::to_wstring((unsigned)ymd.day()) + L"日";

	// 文字列を返す
	return ymdString;
}
