#include "../../Application.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Game/CharacterManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "TaskMessage.h"

TaskMessage::TaskMessage(const Json& param) :
	MOVE_TIME(param["moveTime"]),
	DISPLAY_TIME(param["displayTime"]),
	GHOST_POS_Y(param["ghostPosY"]),
	TEXT_POS_Y(param["textPosY"]),
	START_POS_X(param["startPosX"]),
	END_POS_X(param["endPosX"]),
	GHOST_TEXT_OFFSET_X(param["ghostTextOffsetX"]),
	FONT_NAME(param["fontName"]),
	FONT_SIZE(param["fontSize"]),
	FONT_THICK(param["fontThick"])
{
	endPosX_ = 0.0f;
	moveStep_ = 0.0f;
	startPosX_ = 0.0f;
	timer_ = nullptr;
	state_ = STATE::WAIT;
	type_ = TYPE::MAX;

	// テキストの登録
	const int size = static_cast<int>(TEXT_NAME_LIST.size());
	for (int i = 0; i < size; i++)
	{
		std::wstring text = UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param[TEXT_NAME_LIST[i]]));
		messageListMap_[static_cast<TYPE>(i)] = text;
	}

	// 状態遷移の登録
	stateChangeMap_.emplace(STATE::WAIT, std::bind(&TaskMessage::ChageeStateWait, this));
	stateChangeMap_.emplace(STATE::DELAY, std::bind(&TaskMessage::ChageeStateDelay, this));
	stateChangeMap_.emplace(STATE::ADMISSION, std::bind(&TaskMessage::ChangeStateAdmission, this));
	stateChangeMap_.emplace(STATE::DISPLAY, std::bind(&TaskMessage::ChangeStateDisplay, this));
	stateChangeMap_.emplace(STATE::EXIT, std::bind(&TaskMessage::ChangeStateExit, this));
}

TaskMessage::~TaskMessage()
{
}

void TaskMessage::Init()
{
	// リソースの設定
	int font = fontMng_.CreateMyFont(resMng_.GetFontName(FONT_NAME), FONT_SIZE, FONT_THICK);

	imgGhost_.handleId = resMng_.GetHandle("ghostImage");
	imgGhost_.pos = { START_POS_X - GHOST_TEXT_OFFSET_X , GHOST_POS_Y };

	strMes_.fontHandle = font;
	strMes_.pos = { START_POS_X, TEXT_POS_Y };
	strMes_.color = UtilityCommon::WHITE;
	strMes_.string = messageListMap_[TYPE::NIGHT_VIEW];

	// タイマーの設定
	timer_ = std::make_unique<Timer>(DISPLAY_TIME);
	timer_->InitCountUp();

	// 状態遷移
	ChangeState(STATE::WAIT);
}

void TaskMessage::Update()
{
	updateFunc_();
}

void TaskMessage::Draw()
{
	// 待機中は表示しない
	if (state_ == STATE::WAIT) { return; }
	
	// ゴースト
	imgGhost_.DrawRota();
	
	// テキスト
	strMes_.Draw();
}

void TaskMessage::ChangeMessageType(const TYPE type, const float delayTime)
{	
	// 種類の設定
	type_ = type;

	// テキストの変更
	strMes_.string = messageListMap_[type_];

	// 遅延時間を設けられている場合
	if (delayTime > 0.0f)
	{
		// タイマーの設定
		timer_->SetGoalTime(delayTime);

		// 状態変更
		ChangeState(STATE::DELAY);
	}
	else
	{
		// 状態変更
		ChangeState(STATE::ADMISSION);
	}
}

void TaskMessage::UpdateDelayt()
{
	if (timer_->CountUp())
	{
		ChangeState(STATE::ADMISSION);
	}
}

void TaskMessage::UpdateAdmission()
{
	Move();
	if (moveStep_ >= MOVE_TIME)
	{
		ChangeState(STATE::DISPLAY);
	}
}

void TaskMessage::UpdateDisplay()
{
	if (timer_->CountUp())
	{
		ChangeState(STATE::EXIT);
	}
}

void TaskMessage::UpdateExit()
{
	Move();
	if (moveStep_ >= MOVE_TIME)
	{
		ChangeState(STATE::WAIT);
	}
}

void TaskMessage::ChangeState(const STATE state)
{
	// 状態
	state_ = state;

	// 更新処理の変更
	stateChangeMap_[state_]();
}

void TaskMessage::ChageeStateWait()
{
	updateFunc_ = std::bind(&TaskMessage::UpdateWait, this);
}

void TaskMessage::ChageeStateDelay()
{
	updateFunc_ = std::bind(&TaskMessage::UpdateDelayt, this);
}

void TaskMessage::ChangeStateAdmission()
{
	updateFunc_ = std::bind(&TaskMessage::UpdateAdmission, this);	

	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::TASK_MESSAGE);

	// 移動初期化
	moveStep_ = 0.0f;
	startPosX_ = START_POS_X;
	endPosX_ = END_POS_X;
}

void TaskMessage::ChangeStateDisplay()
{
	updateFunc_ = std::bind(&TaskMessage::UpdateDisplay, this);

	// 表示時間の設定
	timer_->SetGoalTime(DISPLAY_TIME);
	timer_->InitCountUp();
}

void TaskMessage::ChangeStateExit()
{
	updateFunc_ = std::bind(&TaskMessage::UpdateExit, this);

	moveStep_ = 0.0f;
	startPosX_ = END_POS_X;
	endPosX_ = START_POS_X;
}

void TaskMessage::Move()
{
	//時間更新
	moveStep_ += scnMng_.GetDeltaTime();

	//座標計算
	imgGhost_.pos.x = static_cast<int>(UtilityCommon::EaseOutQuad(moveStep_, MOVE_TIME, static_cast<float>(startPosX_ - GHOST_TEXT_OFFSET_X), static_cast<float>(endPosX_ - GHOST_TEXT_OFFSET_X)));
	strMes_.pos.x = static_cast<int>(UtilityCommon::EaseOutQuad(moveStep_, MOVE_TIME, static_cast<float>(startPosX_), static_cast<float>(endPosX_)));
}
