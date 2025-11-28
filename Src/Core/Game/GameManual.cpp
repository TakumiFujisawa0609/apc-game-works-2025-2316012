#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Utility/UtilityCommon.h"
#include "GameManual.h"

GameManual::GameManual()
{
	isText_ = false;
}

GameManual::~GameManual()
{
}

void GameManual::Init()
{	
	// フォントの生成
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, FONT_THICK);

	// テキスト設定
	textExplan_.string = EXPLAN;
	textExplan_.pos = { 0, Application::SCREEN_SIZE_Y - FONT_SIZE * 9 };
	textExplan_.fontHandle = font;
	textExplan_.color = UtilityCommon::WHITE;

	textInput_.string = KEY;
	textInput_.pos = { 175, Application::SCREEN_SIZE_Y - FONT_SIZE * 9 };
	textInput_.fontHandle = font;
	textInput_.color = UtilityCommon::WHITE;

	textOnOff_.string = TEXT_ON_KEY;
	textOnOff_.pos = { 0, Application::SCREEN_SIZE_Y - FONT_SIZE };
	textOnOff_.fontHandle = font;
	textOnOff_.color = UtilityCommon::WHITE;

	// 初期状態は表示状態
	isText_ = true;

	// 初期時の接続状況を確認
	Update();
}

void GameManual::Update()
{
	// パッドの接続状況に応じてテキストを変更
	if (GetJoypadNum() == 0)
	{
		// キーボード用のテキストの反映
		textInput_.string = KEY;
		textOnOff_.string = TEXT_ON_KEY;
	}
	else
	{
		// パッド用のテキストの反映
		textInput_.string = PAD;
		textOnOff_.string = TEXT_ON_PAD;
	}

	// 入力確認
	if (inputMng_.IsTrgDown(InputManager::TYPE::INPUT_EXPLANTION_OPEN))
	{
		// 現在の状態を切り替え
		isText_ = !isText_;
	}
}

void GameManual::Draw()
{
	// 操作説明を開いている場合
	if (isText_)
	{
		textExplan_.Draw();
		textInput_.Draw();
	}
	else
	{
		textOnOff_.Draw();
	}
}
