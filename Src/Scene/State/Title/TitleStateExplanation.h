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
	/// <param name="parent">親インスタンス</param>
	/// <param name="param">パラメーター情報</param>
	TitleStateExplanation(SceneTitle& parent, const Json& param);

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
	std::vector<std::wstring> textList_;

	// フォントサイズ
	const int MAIN_FONT_SIZE;
	const int SKIP_FONT_SIZE;

	// テキストアニメーション速度
	const float TEXT_ANIMATION_SPEED;

	// 説明スキップテキスト位置
	const int SKIP_TEXT_POS_X;
	const int SKIP_TEXT_POS_Y;

	// 背景のサイズ
	const int BACK_BOX_SIZE_X;
	const int BACK_BOX_SIZE_Y;

	// 背景の位置
	const int BACK_BOX_POS_X;
	const int BACK_BOX_POS_Y;

	// 背景のアルファ値
	const int BACK_BOX_ALPHA;

	// 待機時間
	const float WAIT_TIME;

	// テキスト表示時間
	const float TEXT_DISPLAY_TIME;

	// アニメーション時間
	const float ANIM_TIME;

	// 移動速度
	const float MOVE_SPEED;

	// メインの終了位置
	const float END_POS_X;

	// 画像生成クールタイム
	const float WINDOW_CREATE_COOL_TIME;

	// 画像を見送るスプライト番号
	const std::vector<int> SKIP_INDEXS;

	// 画像生成拡大サイズ
	const std::vector<float> IMG_SCALES;

	// スキップ用テキスト
	const std::wstring SKIP_TEXT;

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