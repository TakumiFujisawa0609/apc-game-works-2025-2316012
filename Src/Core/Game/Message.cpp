#include "../../Application.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Game/CharacterManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Object/Actor/Character/Player.h"
#include "../Common/Timer.h"
#include "../Common/ControllerTextAnimation.h"
#include "Message.h"

Message::Message(const Json& param) :
	FONT_SIZE(param["fontSize"]),
	FONT_THICK(param["fontThick"]),
	FONT_NAME(param["fontName"]),
	DISPLAY_TIME(param["displayTime"]),
	BACK_ALPHA(param["backAlpha"]),
	TEXT_ANIMATION_SPEED(param["textAnimationSpeed"]),
	BACK_POS{ param["backPos"]["x"], param["backPos"]["y"] },
	BACK_SIZE{ param["backSize"]["x"], param["backSize"]["y"] }
{
	// テキストの登録
	const int size = static_cast<int>(TEXT_NAME_LIST.size());
	for (int i = 0; i < size; i++)
	{
		const auto& textArray = param[TEXT_NAME_LIST[i]];
		for (auto& text : textArray)
		{
			std::wstring wstr = UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(text));
			messageListMap_[static_cast<TYPE>(i)].emplace_back(wstr);
		}
	}

	messageCnt_ = -1;
	type_ = TYPE::NONE;
	text_ = CharacterString();
	backBoxPos_ = Vector2();
	backBoxSize_ = Vector2();
	textAnimation_ = nullptr;
	timer_ = nullptr;
}

Message::~Message()
{
}

void Message::Load()
{
	int font = FontManager::GetInstance().CreateMyFont(resMng_.GetFontName(FONT_NAME), FONT_SIZE, FONT_THICK);
	const Vector2 POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - FONT_SIZE / 2 };	
	
	// ゲーム開始時のメッセージ
	type_ = TYPE::START;	
	
	// テキストの設定
	text_.string = messageListMap_.at(type_).front();
	text_.fontHandle = font;
	text_.pos = POS;
	text_.color = UtilityCommon::WHITE;

	// 背景の設定
	backBoxPos_ = BACK_POS;
	backBoxSize_ = BACK_SIZE;

	// タイマーの生成
	timer_ = std::make_unique<Timer>(DISPLAY_TIME);

	// アニメーションの初期化
	textAnimation_ = std::make_unique<ControllerTextAnimation>(text_, TEXT_ANIMATION_SPEED);
}

void Message::Init()
{	
	// テキストのアニメーション初期化
	textAnimation_->Init();

	// タイマーの初期化
	timer_->InitCountUp();

	// カウントの初期化
	messageCnt_ = 0;
}

void Message::Update()
{
	// アニメーションを終えている場合
	if (textAnimation_->IsEnd())
	{
		// まだ数秒表示させるためタイマーの更新
		if (timer_->CountUp())
		{
			// テキストのカウントを更新
			messageCnt_++;

			// テキストがまだ残っている場合
			if (messageCnt_ < static_cast<int>(messageListMap_.at(type_).size()))
			{
				// 次のテキストに変更
				text_.string = messageListMap_.at(type_)[messageCnt_];

				// テキストアニメーションの再設定
				textAnimation_->SetCharacterString(text_);

				// タイマーの初期化
				timer_->InitCountUp();
			}
			else
			{
				// 状態変更
				type_ = TYPE::NONE;
			}
		}

		return;
	}

	// テキストアニメーションの更新
	textAnimation_->Update();
}

void Message::Draw()
{
	if (type_ == TYPE::NONE) { return; }

	// 背景の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_ALPHA);
	DrawBox(backBoxPos_.x, backBoxPos_.y, backBoxPos_.x + backBoxSize_.x, backBoxPos_.y + backBoxSize_.y, UtilityCommon::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキストの描画
	textAnimation_->Draw();
}

void Message::ChangeMessage(const TYPE type)
{
	// 種類変更
	type_ = type;

	// テキストの中身を変更
	text_.string = messageListMap_.at(type_).front();

	// テキストアニメーションの再設定
	textAnimation_->SetCharacterString(text_);

	// タイマーの初期化
	timer_->InitCountUp();

	// メッセージカウント初期化
	messageCnt_ = 0;
}