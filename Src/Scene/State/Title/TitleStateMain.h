#pragma once
#include "TitleStateBase.h"
#include "../../Common/CharacterString.h"

class TitleLogo;
class TitleButton;
class TitleOrb;
class PostEffectRipples;

class TitleStateMain : public TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleStateMain(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleStateMain() override;

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
		NONE,
		WAIT,		// 待機
		SELECT,		// 選択
		FADE_IN,	// フェードイン
		FADE_OUT,	// フェードアウト
		EFFECT,		// エフェクト
	};

	enum class MENU_TYPE
	{
		START,			// 開始
		EXPLANATION,	// 説明
		TIPS,			// TIPS
		//SETTING,		// 設定
		EXIT,			// 戻る
		MAX,
	};

	const std::vector<std::wstring> MENU_STRINGS =
	{
		L"開始",
		L"遊び方",
		L"ヒント",
		//L"設定",
		L"戻る",
	};

	// メニュー項目数
	static constexpr int MENU_MAX = static_cast<int>(MENU_TYPE::MAX);

	// フォントサイズ
	static constexpr int FONT_SIZE = 36;
	
	// フォントの太さ
	static constexpr int FONT_THICK = 5;

	// ボックスのパディング
	static constexpr int BOX_PADDING_X = 40;
	static constexpr int BOX_PADDING_Y = 16;

	// フェード速度
	static constexpr float FADE_ALPHA_RATE = 3.0f;

	// エフェクト用透過率減少速度
	static constexpr float EFFECT_ALPHA_RATE = 1.39f;

	// ポストエフェクト用スクリーン
	int effectScreen_;

	// 透過率
	float screenAlpha_;

	// ステップ
	float step_;

	// メニューインデックス
	int menuIndex_;

	// 文字列
	CharacterString menuTexts_[MENU_MAX];

	// ボックス
	Vector2 boxPos_;
	Vector2 boxSize_;

	// 状態
	STATE state_;

	// フェード用次回の遷移状態
	STATE nextState_;

	// 更新処理
	std::function<void()> update_;

	// 描画処理
	std::function<void()> draw_;

	// メニュー別処理
	std::unordered_map<MENU_TYPE, std::function<void()>> menuChangeMap_;

	// 状態遷移管理
	std::unordered_map<STATE, std::function<void()>> stateChangeMap_;

	// ロゴ
	std::unique_ptr<TitleLogo> logo_;

	// ボタン
	std::unique_ptr<TitleButton> button_;

	// オーブ
	std::unique_ptr<TitleOrb> orb_;

	// 波紋エフェクト
	std::unique_ptr<PostEffectRipples> ripples_;

	// 各種更新処理
	void UpdateNone() {};
	void UpdateWait();
	void UpdateSelect();
	void UpdateEffect();
	void UpdateFadeIn();
	void UpdateFadeOut();

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStateWait();
	void ChangeStateSelect();
	void ChangeStateFadeIn();
	void ChangeStateFadeOut();
	void ChangeStateEffect();

	// 各種メニュー項目別処理
	void ChangeMenuStart();
	void ChangeMenuExplanation();
	void ChangeMenuTips();
	void ChangeMenuSetting();
	void ChangeMenuExit();

	// 描画処理
	void DrawWait();
	void DrawSelect();

	// 選択中のテキストの色を変更する
	void UpdateSelectTextColor();

	// ボックスの更新
	void UpdateBox();
};