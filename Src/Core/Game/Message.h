#pragma once
#include <memory>
#include "CoreGameBase.h"

class Timer;
class ControllerTextAnimation;

class Message : public CoreGameBase
{
public:

	/// <summary>
	/// メッセージの種類
	/// </summary>
	enum class TYPE
	{
		NONE,
		START,
		ENEMY_RESPOWN,
		MADNESS,
		MADNESS_PINCH,
		ONE_MINNUTES_LATER,
		REPORT_COMPLITE,
		REPORT_MISS,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Message(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Message() override;

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
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// メッセージの種類を変更
	/// </summary>
	/// <param name="type">メッセージの種類</param>
	void ChangeMessage(const TYPE type);

private:

	// テキスト名リスト
	const std::vector<std::string> TEXT_NAME_LIST =
	{
		"textNone",
		"textStart",		
		"textRespown",
		"textMadness",
		"textMadnessPinch",		
		"textOneMinnutesLater",
		"textReportComplite",
		"textMiss"
	};

	// フォントサイズ
	const int FONT_SIZE;

	// フォント太さ
	const int FONT_THICK;

	// メッセージの表示時間(全ての文字を表示後の時間)
	const float DISPLAY_TIME;

	// テキストアニメーション速度
	const float TEXT_ANIMATION_SPEED;

	// 背景透過値
	const int BACK_ALPHA;

	// フォント名
	const std::string FONT_NAME;

	// 背景位置
	const Vector2 BACK_POS;

	// 背景サイズ
	const Vector2 BACK_SIZE;

	// メッセージ数
	int messageCnt_;

	// テキスト種類
	TYPE type_;

	// テキスト
	CharacterString text_;

	// 背景用座標
	Vector2 backBoxPos_;

	// 背景サイズ
	Vector2 backBoxSize_;

	// テキストアニメーションコントローラー
	std::unique_ptr<ControllerTextAnimation> textAnimation_;

	// メッセージリスト管理マップ
	std::unordered_map<TYPE, std::vector<std::wstring>> messageListMap_;

	// タイマー
	std::unique_ptr<Timer> timer_;
};