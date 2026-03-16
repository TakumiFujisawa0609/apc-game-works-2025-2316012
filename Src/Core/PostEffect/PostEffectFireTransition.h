#pragma once
#include "PostEffectBase.h"

class PostEffectFireTransition : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pram">パラメーター</param>
	PostEffectFireTransition(const Json& pram);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectFireTransition() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// ボケレベル
	const float BLUR_LEVEL;

	// 時間
	float time_;
};

