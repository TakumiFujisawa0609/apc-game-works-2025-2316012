#include "../Application.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Utility/UtilityCommon.h"
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
	int titleFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 52, 6);
	int messageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 24, 4);

	// 各種テキストの設定
	titleText_.fontHandle = titleFont;
	titleText_.color = UtilityCommon::WHITE;
	titleText_.pos = { Application::SCREEN_HALF_X, 100 };
	titleText_.string = STATE_TITLES.at(STATE::SELECT);

	messageText_.fontHandle = messageFont;
	messageText_.color = UtilityCommon::WHITE;
	messageText_.pos = { Application::SCREEN_HALF_X, 650 };
	messageText_.string = STATE_MESSAGES.at(STATE::SELECT)[selectIndex_];

	pageText_.fontHandle = messageFont;
	pageText_.color = UtilityCommon::WHITE;
	pageText_.pos = { 1200, Application::SCREEN_SIZE_Y - 24 };
	pageText_.string = L"%d/%d";
	pageText_.data1 = 1;
	pageText_.data2 = TOTAL_PAGE_COUNT;

	// 各種テクスチャの設定
	// 背景
	background_.handleId = resMng_.GetHandle("sceneBack");
	background_.pos = { 0,0 };
	background_.size = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	// 操作説明アイコン
	operationIcon_.handleId = resMng_.GetHandle("operationExplanationIcon");
	operationIcon_.pos = { Application::SCREEN_HALF_X + 250, Application::SCREEN_HALF_Y };

	// 遊び方説明アイコン
	playIcon_.handleId = resMng_.GetHandle("playExplanationIcon");
	playIcon_.pos = { Application::SCREEN_HALF_X - 250, Application::SCREEN_HALF_Y };

	// 選択フレーム
	selectFrame_.handleId = resMng_.GetHandle("selectFrame");
	selectFrame_.pos = playIcon_.pos;

	// 操作説明
	operationExpanation_.handleId = resMng_.GetHandle("keyboardExplanation");
	operationExpanation_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// 切り替えアイコン
	constexpr int CHANGE_ICON_POS_X[] = { Application::SCREEN_HALF_X - 450,Application::SCREEN_HALF_X + 450 };
	constexpr float CHANGE_ICON_ANGLE_DEG[] = { 0.0f,180.0f };
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
	update_();
}

void SceneExplanation::NormalDraw()
{
	// 背景描画
	background_.DrawExtend();

	// 見出しの描画
	titleText_.DrawCenter();

	// 吹き出しメッセージの描画
	messageText_.DrawCenter();

	// 状態別描画
	draw_();
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
	//

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
	//

	// ページ数の更新
	pageText_.data1 = pageCount_ + 1;
}