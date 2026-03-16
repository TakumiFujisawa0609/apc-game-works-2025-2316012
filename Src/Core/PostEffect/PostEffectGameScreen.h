#pragma once
#include "PostEffectBase.h"

class PostEffectGameScreen : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター情報</param>
	PostEffectGameScreen(const Json& param);

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
	const int BLUR_CONDITION;

	//ブラーを掛ける最大距離数
	const float MAX_BLUR_DISTANCE;

	// フラッシュの最大強度
	const float MAX_FLASH_POWER;

	//ブラー速度
	const float BLUR_SPEED;

	// フラッシュ強さ
	float flashPower_;

	//ブラー用ステップ
	float blurStep_;

	//反転用
	float invertStep_;
};

