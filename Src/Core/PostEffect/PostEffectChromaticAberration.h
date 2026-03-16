#pragma once
#include "PostEffectBase.h"
#include "../../Common/Vector2F.h"

class PostEffectChromaticAberration : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター情報</param>
	PostEffectChromaticAberration(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectChromaticAberration() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 色差分調整
	const Vector2F COLOR_OFFSET = { 0.008f,0.002f };
};

