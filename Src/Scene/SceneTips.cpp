#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Core/Common/SceneBackGround.h"
#include "SceneTips.h"

SceneTips::SceneTips()
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneTips::NormalUpdate, this);

	//描画関数のセット
	drawFunc_ = std::bind(&SceneTips::NormalDraw, this);

	// 初期化
	pageCount_ = 0;

	Init();
}

SceneTips::~SceneTips()
{
}

void SceneTips::Init()
{
	// ページ数の初期化
	pageCount_ = 0;

	// フォント作成
	int titleFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_TITLE, FONT_THICK_TITLE);
	int messageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_MESSAGE, FONT_THICK_COMMON);
	int screenOperationMessageFont = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE_OPERATION, FONT_THICK_COMMON);

	// 各種テキストの設定
	titleText_.fontHandle = titleFont;
	titleText_.color = UtilityCommon::WHITE;
	titleText_.pos = { Application::SCREEN_HALF_X, TIPS_TITLE_POS_Y };
	titleText_.string = TITLE_MESSAGE;

	messageText_.fontHandle = messageFont;
	messageText_.color = UtilityCommon::WHITE;
	messageText_.pos = { Application::SCREEN_HALF_X, TIPS_MESSAGE_POS_Y };
	messageText_.string = TIPS_MESSAGES[pageCount_];

	pageText_.fontHandle = messageFont;
	pageText_.color = UtilityCommon::WHITE;
	pageText_.pos = { TIPS_PAGE_POS_X, TIPS_UI_POS_Y };
	pageText_.string = L"%d/%d";
	pageText_.data1 = 1;
	pageText_.data2 = TIPS_COUNT;

	screenOperationMessage_.fontHandle = screenOperationMessageFont;
	screenOperationMessage_.color = UtilityCommon::WHITE;
	screenOperationMessage_.pos = { 0, TIPS_UI_POS_Y };
	screenOperationMessage_.string = GetOperationMessage();

	// 各種テクスチャの設定
	// 切り替えアイコン
	constexpr int CHANGE_ICON_POS_X[] =
	{
		TIPS_CHANGE_ICON_LEFT_X,
		TIPS_CHANGE_ICON_RIGHT_X
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

	// 説明画像
	tipsMaterial_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("tipsShader"), 1);
	tipsRenderer_ = std::make_unique<PixelRenderer>(*tipsMaterial_);

	// マテリアル
	tipsMaterial_->AddTextureBuf(resMng_.GetHandle(RESOURCE_NAMES[pageCount_]));
	tipsMaterial_->AddConstBuf(FLOAT4{ TIPS_SHADER_CENTER_X, TIPS_SHADER_CENTER_Y, 0.0f, 0.0f });

	// レンダラー
	tipsRenderer_->SetPos({ TIPS_IMAGE_POS_X, TIPS_IMAGE_POS_Y });
	tipsRenderer_->SetSize({ TIPS_IMAGE_WIDTH, TIPS_IMAGE_HEIGHT });
	tipsRenderer_->MakeSquereVertex();

	// 背景
	backGround_ = std::make_unique<SceneBackGround>();
	backGround_->Init();
}

void SceneTips::NormalUpdate()
{
	// テキストの切り替え
	screenOperationMessage_.string = GetOperationMessage();

	// 入力確認
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
	{
		// ページ数増加
		pageCount_ = UtilityCommon::WrapStepIndex(pageCount_, 1, 0, TIPS_COUNT);

		// ページの更新
		UpdatePage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		// ページ数減少
		pageCount_ = UtilityCommon::WrapStepIndex(pageCount_, -1, 0, TIPS_COUNT);

		// ページの更新
		UpdatePage();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	// シーンを戻す
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{	
		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		
		// シーンを戻す
		scnMng_.PopScene();
		return;
	}
}

void SceneTips::NormalDraw()
{
	// 背景描画
	backGround_->Draw();

	// 見出しの描画
	titleText_.DrawCenter();

	// 吹き出しメッセージの描画
	messageText_.DrawCenter();

	// ページ数の描画
	pageText_.DrawFormatCenter();

	// 切り替えアイコン描画
	for (int i = 0; i < CHANGE_ICON_MAX; i++)
	{
		changeIcon_[i].DrawRota();
	}

	// ヒントの描画
	tipsRenderer_->Draw();

	// 画面操作説明
	screenOperationMessage_.Draw();
}

void SceneTips::UpdatePage()
{
	// ページ数の更新
	pageText_.data1 = pageCount_ + 1;

	// ヒントメッセージの更新
	messageText_.string = TIPS_MESSAGES[pageCount_];

	// マテリアル
	tipsMaterial_->SetTextureBuf(0, resMng_.GetHandle(RESOURCE_NAMES[pageCount_]));
}

const std::wstring SceneTips::GetOperationMessage() const
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