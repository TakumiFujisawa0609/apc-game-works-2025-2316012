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
		ONE_MINNUTES_LATER,
		REPORT_COMPLITE,
		REPORT_MISS,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Message();

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

	// キャラクターの名前リスト
	const std::unordered_map<TYPE, std::vector<std::wstring>> MESSAGE_LIST =
	{
		{TYPE::NONE,{L""}},
		{TYPE::START,{L"始業の時間になりました",L"調査を開始してください"} },
		{TYPE::ONE_MINNUTES_LATER,{ L"異変が起きやすい時刻になりました", L"十分に警戒して調査を行ってください"}},
		{TYPE::ENEMY_RESPOWN,{ L"危険な生命体の反応を感知しました", L"見つけ次第直ちに逃げてください"}},
		{TYPE::MADNESS,{ L"精神状態が不安定になっています", L"異変を報告して回復してください"}},
		{TYPE::REPORT_COMPLITE,{L"異変が報告されました"}},
		{TYPE::REPORT_MISS,{L"誤った報告がされました"}},
	};

	// メッセージの表示時間(全ての文字を表示後の時間)
	static constexpr float DISPLAY_TIME = 3.0f;

	// 背景透過値
	static constexpr int BACK_ALPHA = 128;

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

	// タイマー
	std::unique_ptr<Timer> timer_;
};