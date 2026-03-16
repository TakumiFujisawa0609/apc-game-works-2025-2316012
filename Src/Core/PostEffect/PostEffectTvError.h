#pragma once
#include "PostEffectBase.h"

class PostEffectTvError : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectTvError(const Json& param);

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

	// 強さ
	const float STRNGTH;
};

