#pragma once
#include "SceneBase.h"
#include "../Common/Sprite.h"
#include "../Common/CharacterString.h"

class SceneTitle : public SceneBase
{

public:

	// コンストラクタ
	SceneTitle(void);

	// デストラクタ
	~SceneTitle(void);

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

private:

	//サウンド
	int se_;
	int bgm_;

	//画像
	Sprite testSprite_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
};
