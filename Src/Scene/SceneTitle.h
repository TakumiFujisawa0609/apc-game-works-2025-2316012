#pragma once
#include "SceneBase.h"
#include "../Common/Sprite.h"
#include "../Common/CharacterString.h"

class TitleLogo;
class PixelMaterial;
class PixelRenderer;

class SceneTitle : public SceneBase
{

public:

	// コンストラクタ
	SceneTitle(void);

	// デストラクタ
	~SceneTitle(void);

	// 読み込み処理
	void Load(void) override;

	// 初期化処理
	void Init(void) override;

private:

	// サウンド
	int se_;				// 効果音
	int bgm_;				// BGM

	// ロゴ
	std::unique_ptr<TitleLogo> logo_;

	// キー画像
	std::unique_ptr<PixelRenderer> keyRenderer_;
	std::unique_ptr<PixelMaterial> keyMaterial_;

	// 更新関数
	void NormalUpdate(void) override;

	// 描画関数
	void NormalDraw(void) override;

	// 処理の変更
	void ChangeNormal(void) override;
};
