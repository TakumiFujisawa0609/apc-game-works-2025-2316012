#pragma once
#include "PostEffectBase.h"

class PostEffectGameScreen : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectGameScreen();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectGameScreen();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// ブラーの条件値
	static constexpr int BLUR_CONDITION = 50;

	//ブラーを掛ける最大距離数
	static constexpr float MAX_BLUR_DISTANCE = 10.0f;

	// フラッシュの最大強度
	static constexpr float MAX_FLASH_POWER = 2.0f;

	//ブラー速度
	static constexpr float BLUR_SPEED = 0.1f;

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 2;

	// フラッシュ強さ
	float flashPower_;

	//ブラー用ステップ
	float blurStep_;

	//反転用
	float invertStep_;
};

