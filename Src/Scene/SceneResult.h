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

	// 状態
	enum class STATE
	{
		START,			// 開始
		DRUM_ROLL,		// ドラムロール
		WAGE_DISPLAY,	// 給与明細の表示	
		END,			// 次の画面遷移
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

	// 賃金詳細
	enum class WAGE_DETAILS
	{
		BASIC,	// 基本
		BONUS,	// ボーナス
		CUT,	// 減給
		MAX
	};
	
	// 賃金詳細種類数
	static constexpr int WAGE_DETAILS_MAX = static_cast<int>(WAGE_DETAILS::MAX);

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

	// 賃金の詳細テキスト
	const std::wstring WAGE_DETAILS_TEXT[WAGE_DETAILS_MAX] =
	{
		L"固定給与：%d円",		// 基本的な賃金
		L"成果手当：%d円",		// 報告によるボーナス
		L"事故控除：%d円",		// 誤報告による減給
	};

	// ゲームオーバー時のコメント
	const std::wstring GAME_OVER_COMMENT = L"死んでしまうとは情けないですね";

	// フォントサイズ
	static constexpr int TITLE_FONT_SIZE = 50;
	static constexpr int SCORE_FONT_SIZE = 120;
	static constexpr int COMMENT_FONT_SIZE = 48;
	static constexpr int WAGE_DETAILS_FONT_SIZE = 28;

	// タイトルテキスト位置
	static constexpr int TITLE_POS_X = 640;
	static constexpr int TITLE_POS_Y = 60;

	// スコアテキスト位置
	static constexpr int SCORE_POS_X = 640;
	static constexpr int SCORE_POS_Y = 150;	
	
	// 給与詳細テキスト位置
	static constexpr int WAGE_DETAILS_POS_X = 640;
	static constexpr int WAGE_DETAILS_POS_Y = 340;

	// コメントテキスト位置
	static constexpr int COMMENT_POS_X = 640;
	static constexpr int COMMENT_POS_Y = 550;

	// ランク位置
	static constexpr int RANK_POS_X = 1050;
	static constexpr int RANK_POS_Y = 550;

	// 装飾用ライン位置
	static constexpr int LINE_POS_X_START = 200;
	static constexpr int LINE_POS_X_GOAL = 1080;
	static constexpr int LINE_POS_Y = 275;

	// ラインの太さ
	static constexpr int LINE_THICKNESS = 5;

	// 開始時間
	static constexpr float START_TIME = 1.0f;

	// テキストアニメーション時間
	static constexpr float TEXT_ANIMATION_TIME = 0.08f;

	// 給与明細表示間隔時間
	static constexpr float WAGE_INTERVAL_TIME = 0.35f;

	// スコア
	int score_;

	// BGM再生判定
	bool isBgm_;

	// 段階的に供与明細を表示する用のインデックス
	int wageIndex_;

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

	// 詳細テキスト
	CharacterString wageDetailsText_[WAGE_DETAILS_MAX];

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
	void UpdateWage();
	void UpdateEnd();
};