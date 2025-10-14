#pragma once
#include "../../CoreBase.h"
#include "../../../Common/CharacterString.h"

class GameManualUi : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManualUi();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManualUi() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

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

	// キーボード操作説明文
	const std::wstring MANUAL_KEY = 
		L"操作方法\n"
		"移動		:WASD\n"
		"ダッシュ	:LShift\n"
		"報告		:R\n"
		"カメラ		:Mouse\n"
		"ポーズ		:P\n"
		;

	// テキスト
	CharacterString text_;
};