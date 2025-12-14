#pragma once
#include "CoreGameBase.h"
#include "../../Common/CharacterString.h"

class GameManual : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManual(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManual() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// フォント名
	const std::string FONT_NAME;

	// フォントサイズ
	const int FONT_SIZE;

	// フォント太さ
	const int FONT_THICK;

	// 説明テキスト位置
	const Vector2 EXPLAN_TEXT_POS;
	
	// 入力テキスト位置
	const Vector2 INPUT_TEXT_POS;

	// 表示確認テキスト位置
	const Vector2 ONOFF_TEXT_POS;

	// 操作説明の表示を促すテキスト
	const std::wstring TEXT_ON_KEY;

	// 操作説明の表示を促すテキスト
	const std::wstring TEXT_ON_PAD;

	// 操作説明文
	const std::wstring EXPLAN;

	// キーボード
	const std::wstring KEY;

	// ゲームパッド
	const std::wstring PAD;

	// テキストの表示判定
	bool isText_;

	// 表示用テキスト
	CharacterString textOnOff_;

	// 説明テキスト
	CharacterString textExplan_;

	// 入力テキスト
	CharacterString textInput_;
};