#pragma once
#include "CoreGameBase.h"
#include "../../Common/CharacterString.h"

class GameManual : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManual();

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

	// 操作説明の表示を促すテキスト
	const std::wstring TEXT_ON_KEY =
		L"操作説明の表示 :Tab"
		;

	// 操作説明の表示を促すテキスト
	const std::wstring TEXT_ON_PAD =
		L"操作説明の表示 :+↓"
		;

	// 操作説明文
	const std::wstring EXPLAN = 
		L"操作方法\n"
		"移動			\n"
		"ダッシュ		\n"
		"報告			\n"
		"カメラ切り替え	\n"
		"カメラ			\n"
		"ポーズ			\n"
		"非表示			\n"		
		;

	// キーボード
	const std::wstring KEY =
		L"\n"
		":WASD\n"
		":LShift\n"
		":LClick\n"
		":E\n"
		":Mouse\n"
		":BackSpace\n"
		":Tab\n"
		;

	// ゲームパッド
	const std::wstring PAD =
		L"\n"
		":LStick\n"
		":A\n"
		":ZR\n"
		":Y\n"
		":RStick\n"
		":Select\n"
		":+↓\n"
		;

	// テキストの表示判定
	bool isText_;

	// 表示用テキスト
	CharacterString textOnOff_;

	// 説明テキスト
	CharacterString textExplan_;

	// 入力テキスト
	CharacterString textInput_;
};