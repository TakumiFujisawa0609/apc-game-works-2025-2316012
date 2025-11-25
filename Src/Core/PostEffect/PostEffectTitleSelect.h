#pragma once

#include "PostEffectBase.h"

class PostEffectTitleSelect : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectTitleSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectTitleSelect() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;

	//ブラーを掛ける距離
	static constexpr float BLUR_DISTANCE = 3.0f;

	// 時間
	float time_;
};

