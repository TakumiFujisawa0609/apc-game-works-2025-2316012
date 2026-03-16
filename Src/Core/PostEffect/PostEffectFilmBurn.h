#pragma once
#include "PostEffectBase.h"

class PostEffectFilmBurn : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター情報</param>
	PostEffectFilmBurn(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectFilmBurn() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 時間
	const float TIME;

	// セピア色
	const VECTOR SEPIA_COLOR;

};

