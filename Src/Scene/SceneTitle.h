#pragma once
#include "SceneBase.h"

class TitleLogo;
class PostEffectRipples;
class PixelMaterial;
class PixelRenderer;

class SceneTitle : public SceneBase
{

public:

	// コンストラクタ
	SceneTitle();

	// デストラクタ
	~SceneTitle();

	// 読み込み処理
	void Load() override;

	// 初期化処理
	void Init() override;

private:

	int effectScreen_;

	// ロゴ
	std::unique_ptr<TitleLogo> logo_;

	// 波紋エフェクト
	std::unique_ptr<PostEffectRipples> ripples_;

	// キー画像
	std::unique_ptr<PixelRenderer> keyRenderer_;
	std::unique_ptr<PixelMaterial> keyMaterial_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;
};
