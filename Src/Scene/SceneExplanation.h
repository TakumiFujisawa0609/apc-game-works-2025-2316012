#pragma once
#include <functional>
#include <unordered_map>
#include "SceneBase.h"
#include "../Common/Image.h"
#include "../Common/CharacterString.h"

class SceneExplanation : public SceneBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneExplanation();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneExplanation() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void NormalUpdate()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void NormalDraw()override;

private:

	// 状態
	enum class STATE
	{
		SELECT,					// 操作説明か遊び方説明か選ぶ画面
		PLAY_EXPLANATION,		// 遊び方説明画面
		OPERATION_EXPLANATION,	// 操作説明画面
	};

	// 各状態の見出し
	const std::unordered_map < STATE, std::wstring> STATE_TITLES =
	{
		{ STATE::SELECT, L"説明を選択してください" },
		{ STATE::OPERATION_EXPLANATION, L"操作説明" },
		{ STATE::PLAY_EXPLANATION, L"遊び方説明" },
	};

	const std::unordered_map < STATE, std::vector<std::wstring>> STATE_MESSAGES =
	{
		{ STATE::SELECT, {
			L"ゲームの遊び方を説明をします", 
			L"キーボードやコントローラーの操作方法を説明します" 
	} },
		{ STATE::OPERATION_EXPLANATION,
		{
			L"キーボードの操作方法",
			L"ゲームパッドの操作方法" 
	} },
		{ STATE::PLAY_EXPLANATION, 
		{ 
			L"本作はサバイバルシミュレーション作品であり、\n5分間生存し続けることが出来ればクリアとなります。",
			L"最初の1分間は後の異変に備え、ステージを探索して\n構造を覚えておきましょう。",
			L"一定時間経過後、ステージ内に異変が発生します。\nステージを探索して様々な異変を見つけましょう。",
			L"異変を見つけたら速やかに報告しましょう。\n異変の報告に成功すると正気度を回復することが出来ます。",
			L"正気度はプレイヤーの体力であり、一定以上まで下がった場合、\n視界が狭まり画面がぼやけてきます。",
			L"正気度が0になった場合、プレイヤーは絶命します。\n異変を報告して、時間まで正気度を保ち続けましょう。",
			L"一定時間経過後、プレイヤーを襲う敵が出現します。",
			L"敵は異変ではなく、プレイヤーを見つけ次第襲い掛かります。\n敵に見つからずに時間まで生き延び続けましょう。",
			L"ゲーム終了後、成績に応じた評価が与えられます。\nたくさん異変を見つけて最高評価を目指しましょう。"
	} },
	};

	// 切り替えアイコンの数
	static constexpr int CHANGE_ICON_MAX = 2;

	// 説明の種類数
	static constexpr int EXPLANATION_TYPE_MAX = 2;

	// 遊び方の総ページ数
	static constexpr int TOTAL_PAGE_COUNT = 9;

	// 現在の状態
	STATE state_;	

	// 選択中の項目
	int selectIndex_;

	// ページ数
	int pageCount_;
	
	// 更新処理
	std::function<void()> update_;

	// 描画処理
	std::function<void()> draw_;

	// 背景
	Image background_;	

	// 操作説明アイコン
	Image operationIcon_;

	// 遊び方説明アイコン
	Image playIcon_;

	// 操作方法説明画像
	Image operationExpanation_;

	// 選択枠
	Image selectFrame_;

	// 切り替えアイコン
	Image changeIcon_[CHANGE_ICON_MAX];

	// 見出し
	CharacterString titleText_;

	// 吹き出しメッセージ
	CharacterString messageText_;

	// ページ数
	CharacterString pageText_;

	// 状態遷移処理の管理
	std::unordered_map<STATE, std::function<void()>> stateChangeMap_;

	// 各状態の更新処理
	void UpdateSelect();
	void UpdateOperationExplanation();
	void UpdatePlayExplanation();

	// 各状態の描画処理
	void DrawSelect();
	void DrawOperationExplanation();
	void DrawPlayExplanation();

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateSelect();
	void ChangeStateOperationExplanation();
	void ChangeStatePlayExplanation();

	// 選択画面でのメッセージの更新
	void UpdateSelectMessage();

	// 操作説明での切り替え処理
	void UpdateChangeOperationExplanation();

	// ページ更新処理
	void UpdatePage();
};