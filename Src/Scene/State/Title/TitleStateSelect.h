#pragma once
#include <array>
#include "TitleStateBase.h"
#include "../../../Common/CharacterString.h"
#include "../../../Common/Image.h"

class PostEffectTitleSelect;
class GlitchScreen;

class TitleStateSelect : public TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleStateSelect(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleStateSelect() override;

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

	// 状態
	enum class STATE
	{
		NONE,
		SELECT,
		START,
		END
	};

	// 選択項目
	enum class TYPE
	{
		START,
		END,
		MAX
	};

	// 選択項目総数
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	// 選択項目
	int type_;

	// 選択後用のステップ
	float afterStep_;

	// エフェクトスクリーン
	int effectScreen_;

	// 選択背景
	Image selectBack_;

	// 選択項目用テキスト
	std::array<CharacterString, TYPE_MAX> selectTexts_;

	// 選択後に表示するテキスト
	CharacterString afterText_;

	// グリッチ
	std::unique_ptr<GlitchScreen> glitch_;

	// エフェクト
	std::unique_ptr<PostEffectTitleSelect> effect_;

	// 項目別処理の管理
	std::unordered_map<TYPE, std::function<void()>> changeMap_;

	// 更新処理
	std::function<void()> update_;

	// 描画処理
	std::function<void()> draw_;

	// 状態遷移処理
	void ChangeStart();
	void ChangeEnd();

	// 状態別更新処理
	void UpdateNone() {};
	void UpdateSelect();
	void UpdateStart();
	void UpdateEnd();

	// 状態別描画処理
	void DrawSelect();
	void DrawAfter();
};