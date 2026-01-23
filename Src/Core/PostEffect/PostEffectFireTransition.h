#pragma once
#include "PostEffectBase.h"

class PostEffectFireTransition : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectFireTransition();

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

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;

	// ボケレベル
	static constexpr float BLUR_LEVEL = 0.1f;

	// 時間
	float time_;
};

