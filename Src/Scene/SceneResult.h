#pragma once
#include "SceneBase.h"
#include "../Common/CharacterString.h"
#include "../Common/Sprite.h"
#include "../Common/Image.h"

class Timer;
class ControllerTextAnimation;

class SceneResult : public SceneBase
{
public:

	enum class STATE
	{
		START,
		DRUM_ROLL,
		END,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneResult();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneResult();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// ランク
	enum class RANK
	{
		S,
		A,
		B,
		C,
		MAX
	};

	// ランク別スコア
	static constexpr int RANK_SCORES[static_cast<int>(RANK::MAX)] = 
	{
		15000, // Sランク
		13000, // Aランク
		12000, // Bランク
		10000  // Cランク
	};

	// ランク別評価コメント
	const std::wstring RANK_COMMENTS[static_cast<int>(RANK::MAX)] =
	{
		L"大変よくできました",				// Sランク
		L"素晴らしい結果です",				// Aランク
		L"良い結果です",					// Bランク
		L"もっと頑張りましょう",			// Cランク
	};

	// ゲームオーバー時のコメント
	const std::wstring GAME_OVER_COMMENT = L"死んでしまうとは情けないですね";

	// フォントサイズ
	static constexpr int TITLE_FONT_SIZE = 48;
	static constexpr int SCORE_FONT_SIZE = 80;
	static constexpr int COMMENT_FONT_SIZE = 32;

	// タイトルテキスト位置
	static constexpr int TITLE_POS_X = 640;
	static constexpr int TITLE_POS_Y = 60;

	// スコアテキスト位置
	static constexpr int SCORE_POS_X = 640;
	static constexpr int SCORE_POS_Y = 300;

	// コメントテキスト位置
	static constexpr int COMMENT_POS_X = 640;
	static constexpr int COMMENT_POS_Y = 500;

	// ランク位置
	static constexpr int RANK_POS_X = 1000;
	static constexpr int RANK_POS_Y = 500;

	// 開始時間
	static constexpr float START_TIME = 1.0f;

	// テキストアニメーション時間
	static constexpr float TEXT_ANIMATION_TIME = 0.08f;

	// スコア
	int score_;

	// BGM再生判定
	bool isBgm_;

	// 状態
	STATE state_;

	// 更新処理
	std::function<void()> update_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// テキストアニメーション
	std::unique_ptr<ControllerTextAnimation> textAnimation_;

	// テキスト
	CharacterString titleText_;

	// スコアテキスト
	CharacterString scoreText_;

	// コメントテキスト
	CharacterString commentText_;

	// ランク
	Sprite rank_;

	// ゴースト
	Image ghost_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	// 状態別更新処理
	void UpdateStart();
	void UpdateDrumRoll();
	void UpdateEnd();
};