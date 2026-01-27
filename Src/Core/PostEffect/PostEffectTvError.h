#pragma once
#include "PostEffectBase.h"

class PostEffectTvError : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectTvError();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectTvError() override;

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

	// 強さ
	static constexpr float STRNGTH = 0.01f;
};

