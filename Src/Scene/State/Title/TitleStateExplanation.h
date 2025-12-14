#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "TitleStateBase.h"
#include "../../../Common/CharacterString.h"
#include "../../../Common/Sprite.h"
#include "../../../Common/Image.h"

class ControllerTextAnimation;
class Timer;

class TitleStateExplanation : public TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleStateExplanation(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleStateExplanation() override;

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

private:

	enum class STATE
	{
		WAIT,
		TEXT_DISPLAY,
		BUG,
	};

	// 表示するテキスト一覧
	std::vector<std::wstring> texts_ = 
	{
		L"従業員の皆さんへ",
		L"これから仕事について説明します",
		L"皆さんにはこの家の中で",
		L"異変探しをしてもらいます",
		L"異変を見つけた際は",
		L"素早く報告してください",
		L"暗視でしか見つけれない異変もあるので",
		L"うまく使い分けてくださいね",
		L"業務中体調が悪化する可能性があります",
		L"もし体調が悪い場合は異変を報告して回復しましょう",
		L"また、異変探索中別生命体に会う場合があります",
		L"それは異変ではないので速やかに逃げましょう",
		L"以上で仕事の説明は終了になります",
		L"皆さんのご健闘を心よりお祈り申し上げます",
	};

	// フォントサイズ
	static constexpr int MAIN_FONT_SIZE = 24;
	static constexpr int SKIP_FONT_SIZE = 48;

	// テキストアニメーション速度
	static constexpr float TEXT_ANIMATION_SPEED = 0.12f;

	// 説明スキップテキスト位置
	static constexpr int SKIP_TEXT_POS_X = 900;
	static constexpr int SKIP_TEXT_POS_Y = 670;

	// 背景のサイズ
	static constexpr int BACK_BOX_SIZE_X = 1280;
	static constexpr int BACK_BOX_SIZE_Y = 60;

	// 背景の位置
	static constexpr int BACK_BOX_POS_X = 0;
	static constexpr int BACK_BOX_POS_Y = 330;

	// 背景のアルファ値
	static constexpr int BACK_BOX_ALPHA = 128;

	// 待機時間
	static constexpr float WAIT_TIME = 1.5f;

	// テキスト表示時間
	static constexpr float TEXT_DISPLAY_TIME = 2.0f;

	//アニメーション時間
	static constexpr float ANIM_TIME = 1.5f;

	// 移動速度
	static constexpr float MOVE_SPEED = 5.0f;

	// メインの終了位置
	static constexpr float END_POS_X = -128;

	// 画像生成クールタイム
	static constexpr float WINDOW_CREATE_COOL_TIME = 0.08f;

	// 画像を見送るスプライト番号
	const std::vector<int> SKIP_INDEXS = { 0, 1, 3, 5, 13 };

	// 画像生成拡大サイズ
	const std::vector<float> IMG_SCALES = { 0.7f, 0.9f, 1.0f, 1.2f, 1.3f };

	// スキップ用テキスト
	const std::wstring SKIP_TEXT = L"Enterでスキップ";

	// スキップテキスト
	CharacterString skipText_;

	// 状態
	STATE state_;

	// 画像ウィンドウ
	int imgWindow_;

	// ウィンドウ生成用ステップ
	float windowCreateStep_;

	// テキストの番号
	int textIndex_;

	// 移動用ステップ
	float moveStep_;

	// スプライトの更新判定
	bool isUpdateSprite_;

	// メインスプライト
	Sprite mainSprite_;

	// サブスプライト
	Sprite subSprite_;

	// 背景用座標
	Vector2 backBoxPos_;
	Vector2 backBoxSize_;

	// テキスト
	CharacterString text_;

	// ウィンドウ配列
	std::vector<Image> windows_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// テキストのアニメーションコントローラー
	std::unique_ptr<ControllerTextAnimation> textAnimationController_;

	// 状態別更新処理管理マップ
	std::unordered_map<STATE, std::function<void()>> stateUpdateMap_;
	
	// 状態別描画処理管理マップ
	std::unordered_map<STATE, std::function<void()>> stateDrawMap_;

	// 処理の登録
	void RegisterStateFunctions(const STATE state, std::function<void()> update, std::function<void()> draw);

	// 各種状態別更新処理
	void UpdateWait();
	void UpdateTextDisplay();
	void UpdateBug();
	void UpdateSprite();

	// 各種状態別描画処理
	void DrawNone() {};
	void DrawTextDisplay();
	void DrawBug();

	// スプライトの変更スキップ判定
	bool IsSkipSpriteChange();
};