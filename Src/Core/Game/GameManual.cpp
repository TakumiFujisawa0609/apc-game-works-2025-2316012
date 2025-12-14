#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Utility/UtilityCommon.h"
#include "GameManual.h"

GameManual::GameManual(const Json& param) :
	FONT_SIZE(param["fontSize"]),
	FONT_THICK(param["fontThick"]),
	FONT_NAME(param["fontName"]),
	EXPLAN(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["explanText"]))),
	KEY(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["keyText"]))),
	PAD(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["padText"]))),
	TEXT_ON_KEY(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["onKeyText"]))),
	TEXT_ON_PAD(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["onPadText"]))),
	EXPLAN_TEXT_POS{ param["explanTextPos"]["x"], param["explanTextPos"]["y"] },
	INPUT_TEXT_POS{ param["inputTextPos"]["x"], param["inputTextPos"]["y"] },
	ONOFF_TEXT_POS{ param["onOffTextPos"]["x"], param["onOffTextPos"]["y"] }
{
	isText_ = false;
}

GameManual::~GameManual()
{
}

void GameManual::Init()
{	
	// フォントの生成
	int font = fontMng_.CreateMyFont(resMng_.GetFontName(FONT_NAME), FONT_SIZE, FONT_THICK);

	// テキスト設定
	textExplan_.string = EXPLAN;
	textExplan_.pos = EXPLAN_TEXT_POS;
	textExplan_.fontHandle = font;
	textExplan_.color = UtilityCommon::WHITE;

	textInput_.string = KEY;
	textInput_.pos = INPUT_TEXT_POS;
	textInput_.fontHandle = font;
	textInput_.color = UtilityCommon::WHITE;

	textOnOff_.string = TEXT_ON_KEY;
	textOnOff_.pos = ONOFF_TEXT_POS;
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
