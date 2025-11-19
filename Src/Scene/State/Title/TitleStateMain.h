#pragma once
#include "TitleStateBase.h"

class TitleLogo;
class TitleButton;
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
		WAIT,	// 待機
		EFFECT, // エフェクト
	};

	// ポストエフェクト用スクリーン
	int effectScreen_;

	// 透過率
	float screenAlpha_;

	// ステップ
	float step_;

	// 更新処理
	std::function<void()> update_;

	// ロゴ
	std::unique_ptr<TitleLogo> logo_;

	// ボタン
	std::unique_ptr<TitleButton> button_;

	// 波紋エフェクト
	std::unique_ptr<PostEffectRipples> ripples_;

	void UpdateWait();
	void UpdateEffect();
};

