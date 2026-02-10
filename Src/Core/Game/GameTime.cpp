#include <chrono>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/ScoreManager.h"
#include "../../Manager/Game/GameSystemManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Game/CharacterManager.h"
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
	EVENT_TIME_LIST{ param["eventTimeList"].begin(), param["eventTimeList"].end() },
	stateMng_(GameStateManager::GetInstance())
{
	eventStep_ = 0;
	isEvent_ = false;
	isEnemyAppear_ = false;
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

	// 初期イベント処理
	eventFunc_ = std::bind(&GameTime::EventStartExplan, this);
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
	
	// イベント実行数が最大まで満たしてない場合
	if (eventStep_ < static_cast<int>(EVENT_TIME_LIST.size()))
	{
		// イベント発生時間を超えた場合
		if (EVENT_TIME_LIST[eventStep_] <= timer_->GetCount())
		{
			// ステップの更新
			eventStep_++;

			// イベント処理の実行
			eventFunc_();
		}
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

void GameTime::EventStartExplan()
{
	// タスクメッセージの表示
	systemMng_.ChangeTaskMessage(TaskMessage::TYPE::START);

	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventNightViewExplan, this);
}

void GameTime::EventNightViewExplan()
{
	// タスクメッセージの表示
	systemMng_.ChangeTaskMessage(TaskMessage::TYPE::NIGHT_VIEW);

	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventSanityExplan, this);
}

void GameTime::EventSanityExplan()
{
	// タスクメッセージの表示
	systemMng_.ChangeTaskMessage(TaskMessage::TYPE::MADNESS);

	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventOneMinutesLater, this);
}

void GameTime::EventOneMinutesLater()
{
	// メッセージの変更
	systemMng_.ChangeMessage(Message::TYPE::ONE_MINNUTES_LATER);

	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventEnemyAppear, this);
}

void GameTime::EventEnemyAppear()
{
	// 敵の生成
	CharacterManager::GetInstance().RespownEnemy();

	// 状態変更
	stateMng_.ChangeState(GameStateManager::STATE::RESPOWN_ENEMY);

	// タスクメッセージの表示
	systemMng_.ChangeTaskMessage(TaskMessage::TYPE::ENEMY, TASK_EVENT_DELAY_TIME);

	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventFinalWarning, this);
}

void GameTime::EventFinalWarning()
{
	// タスクメッセージの表示
	systemMng_.ChangeTaskMessage(TaskMessage::TYPE::FINAL_WARNING);	
	
	// 次のイベント設定
	eventFunc_ = std::bind(&GameTime::EventNone, this);
}
