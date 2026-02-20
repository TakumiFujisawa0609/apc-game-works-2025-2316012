#include "../Application.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Core/Common/SceneBackGround.h"
#include "SceneExplanation.h"

SceneExplanation::SceneExplanation()
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneExplanation::NormalUpdate, this);

	//描画関数のセット
	drawFunc_ = std::bind(&SceneExplanation::NormalDraw, this);

	// 状態遷移時の処理を登録
	stateChangeMap_.emplace(STATE::SELECT, std::bind(&SceneExplanation::ChangeStateSelect, this));
	stateChangeMap_.emplace(STATE::OPERATION_EXPLANATION, std::bind(&SceneExplanation::ChangeStateOperationExplanation, this));
	stateChangeMap_.emplace(STATE::PLAY_EXPLANATION, std::bind(&SceneExplanation::ChangeStatePlayExplanation, this));

	// 初期化
	selectIndex_ = 0;
	pageCount_ = 0;
	Init();
}

SceneExplanation::~SceneExplanation()
{
}

void SceneExplanation::Init()
{
	// フォント作成
	int titleFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_TITLE, FONT_THICK_TITLE);
	int messageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_MESSAGE, FONT_THICK_COMMON);
	int screenOperationMessageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_OPERATION, FONT_THICK_COMMON);

	// 各種テキストの設定
	titleText_.fontHandle = titleFont;
	titleText_.color = UtilityCommon::WHITE;
	titleText_.pos = { Application::SCREEN_HALF_X, SCREEN_Y_TOP_MARGIN };
	titleText_.string = STATE_TITLES.at(STATE::SELECT);

	messageText_.fontHandle = messageFont;
	messageText_.color = UtilityCommon::WHITE;
	messageText_.pos = { Application::SCREEN_HALF_X, MESSAGE_TEXT_POS_Y };
	messageText_.string = STATE_MESSAGES.at(STATE::SELECT)[selectIndex_];

	pageText_.fontHandle = messageFont;
	pageText_.color = UtilityCommon::WHITE;
	pageText_.pos = { PAGE_TEXT_POS_X, Application::SCREEN_SIZE_Y - SCREEN_Y_BOTTOM_MARGIN };
	pageText_.string = L"%d/%d";
	pageText_.data1 = 1;
	pageText_.data2 = TOTAL_PAGE_COUNT;

	screenOperationMessage_.fontHandle = screenOperationMessageFont;
	screenOperationMessage_.color = UtilityCommon::WHITE;
	screenOperationMessage_.pos = { 0, Application::SCREEN_SIZE_Y - SCREEN_Y_BOTTOM_MARGIN };
	screenOperationMessage_.string = GetOperationMessage();


	// 各種テクスチャの設定
	// 背景
	backGround_ = std::make_unique<SceneBackGround>();
	backGround_->Init();

	// 操作説明アイコン
	operationIcon_.handleId = resMng_.GetHandle("operationExplanationIcon");
	operationIcon_.pos = { Application::SCREEN_HALF_X + OPERATION_ICON_OFFSET_X, Application::SCREEN_HALF_Y };

	// 遊び方説明アイコン
	playIcon_.handleId = resMng_.GetHandle("playExplanationIcon");
	playIcon_.pos = { Application::SCREEN_HALF_X - PLAY_ICON_OFFSET_X, Application::SCREEN_HALF_Y };

	// 選択フレーム
	selectFrame_.handleId = resMng_.GetHandle("selectFrame");
	selectFrame_.pos = playIcon_.pos;

	// 遊び方説明
	playExpanation_.handleId = resMng_.GetHandle(RESOURCE_NAMES[pageCount_]);
	playExpanation_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// 操作説明
	operationExpanation_.handleId = resMng_.GetHandle("keyboardExplanation");
	operationExpanation_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// 切り替えアイコン
	constexpr int CHANGE_ICON_POS_X[] =
	{
		Application::SCREEN_HALF_X - CHANGE_ICON_OFFSET_X,
		Application::SCREEN_HALF_X + CHANGE_ICON_OFFSET_X
	};
	constexpr float CHANGE_ICON_ANGLE_DEG[] =
	{
		0.0f,
		ANGLE_FLIP_DEG
	};

	for (int i = 0; i < CHANGE_ICON_MAX; i++)
	{
		changeIcon_[i].handleId = resMng_.GetHandle("changeIcon");
		changeIcon_[i].pos = { CHANGE_ICON_POS_X[i], Application::SCREEN_HALF_Y };
		changeIcon_[i].angle = UtilityCommon::Deg2RadF(CHANGE_ICON_ANGLE_DEG[i]);
	}

	// 初期状態へ遷移
	ChangeState(STATE::SELECT);
}

void SceneExplanation::NormalUpdate()
{
	// パッドの接続状況を確認して、画面の操作方法を更新
	screenOperationMessage_.string = GetOperationMessage();

	// 状態別更新
	update_();
}

void SceneExplanation::NormalDraw()
{
	// 背景描画
	backGround_->Draw();

	// 見出しの描画
	titleText_.DrawCenter();

	// 吹き出しメッセージの描画
	messageText_.DrawCenter();

	// 状態別描画
	draw_();

	// 画面の操作方法の描画
	screenOperationMessage_.Draw();
}

void SceneExplanation::UpdateSelect()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 状態遷移
		ChangeState(static_cast<STATE>(selectIndex_ + 1));

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
	{
		// 増加
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, 1, 0, EXPLANATION_TYPE_MAX);

		// メッセージの更新
		UpdateSelectMessage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		// 減少
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, -1, 0, EXPLANATION_TYPE_MAX);

		// メッセージの更新
		UpdateSelectMessage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	// シーンを戻す
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);	
		
		scnMng_.PopScene();
		return;
	}

	// フレーム位置の更新
	selectFrame_.pos = (selectIndex_ == 0) ? playIcon_.pos : operationIcon_.pos;
}

void SceneExplanation::UpdateOperationExplanation()
{
	// 状態を戻す
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		ChangeState(STATE::SELECT);

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}

	// 接続状況に応じて更新
	UpdateChangeOperationExplanation();
}

void SceneExplanation::UpdatePlayExplanation()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
	{
		// ページ数増加
		pageCount_ = UtilityCommon::WrapStepIndex(pageCount_, 1, 0, TOTAL_PAGE_COUNT);

		// ページの更新
		UpdatePage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		// ページ数減少
		pageCount_ = UtilityCommon::WrapStepIndex(pageCount_, -1, 0, TOTAL_PAGE_COUNT);

		// ページの更新
		UpdatePage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	// シーンを戻す
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		// 状態遷移
		ChangeState(STATE::SELECT);

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}
}

void SceneExplanation::DrawSelect()
{
	// 操作説明アイコン描画
	operationIcon_.DrawRota();

	// 遊び方説明アイコン描画
	playIcon_.DrawRota();

	// 選択枠描画
	selectFrame_.DrawRota();
}

void SceneExplanation::DrawOperationExplanation()
{
	// 操作方法を描画
	operationExpanation_.DrawRota();
}

void SceneExplanation::DrawPlayExplanation()
{
	// 遊び方説明描画
	playExpanation_.DrawRota();

	// 切り替えアイコン描画
	for (int i = 0; i < CHANGE_ICON_MAX; i++)
	{
		changeIcon_[i].DrawRota();
	}

	// ページ数描画
	pageText_.DrawFormatCenter();
}

void SceneExplanation::ChangeState(const STATE state)
{
	state_ = state;

	stateChangeMap_[state_]();

	titleText_.string = STATE_TITLES.at(state_);
}

void SceneExplanation::ChangeStateSelect()
{
	update_ = std::bind(&SceneExplanation::UpdateSelect, this);
	draw_ = std::bind(&SceneExplanation::DrawSelect, this);

	// 初期化
	UpdateSelectMessage();
}

void SceneExplanation::ChangeStateOperationExplanation()
{
	update_ = std::bind(&SceneExplanation::UpdateOperationExplanation, this);
	draw_ = std::bind(&SceneExplanation::DrawOperationExplanation, this);

	// 初期化
	UpdateChangeOperationExplanation();
}

void SceneExplanation::ChangeStatePlayExplanation()
{
	update_ = std::bind(&SceneExplanation::UpdatePlayExplanation, this);
	draw_ = std::bind(&SceneExplanation::DrawPlayExplanation, this);

	// ページ数の初期化
	pageCount_ = 0;
	messageText_.string = STATE_MESSAGES.at(STATE::PLAY_EXPLANATION)[pageCount_];
}

void SceneExplanation::UpdateSelectMessage()
{
	// メッセージの更新
	messageText_.string = STATE_MESSAGES.at(STATE::SELECT)[selectIndex_];
}

void SceneExplanation::UpdateChangeOperationExplanation()
{
	// コントローラーまたはキーボードで分岐
	if (GetJoypadNum() == 0)
	{
		operationExpanation_.handleId = resMng_.GetHandle("keyboardExplanation");
		messageText_.string = STATE_MESSAGES.at(STATE::OPERATION_EXPLANATION)[0];
	}
	else
	{
		operationExpanation_.handleId = resMng_.GetHandle("controllerExplanation");
		messageText_.string = STATE_MESSAGES.at(STATE::OPERATION_EXPLANATION)[1];
	}
}

void SceneExplanation::UpdatePage()
{
	// 説明文の更新
	messageText_.string = STATE_MESSAGES.at(STATE::PLAY_EXPLANATION)[pageCount_];
	
	// 説明画像の更新
	playExpanation_.handleId = resMng_.GetHandle(RESOURCE_NAMES[pageCount_]);

	// ページ数の更新
	pageText_.data1 = pageCount_ + 1;
}

const std::wstring SceneExplanation::GetOperationMessage() const
{
	if (GetJoypadNum() > 0)
	{
		return OPERATION_MESSAGE_PAD;
	}
	else
	{
		return OPERATION_MESSAGE_KEY;
	}
}
