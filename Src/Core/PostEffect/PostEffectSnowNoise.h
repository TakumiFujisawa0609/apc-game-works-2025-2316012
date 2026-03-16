#pragma once
#include "PostEffectBase.h"

class PostEffectSnowNoise : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター情報</param>
	PostEffectSnowNoise(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectSnowNoise() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	//ブラーを掛ける距離
	const float BLUR_DISTANCE;

	// ノイズ用ステップ
	float noiseStep_;
};