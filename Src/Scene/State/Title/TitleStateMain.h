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

	// ポストエフェクト用スクリーン
	int effectScreen_;

	// ロゴ
	std::unique_ptr<TitleLogo> logo_;

	// ボタン
	std::unique_ptr<TitleButton> button_;

	// 波紋エフェクト
	std::unique_ptr<PostEffectRipples> ripples_;
};

