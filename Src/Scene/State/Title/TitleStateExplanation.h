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