#pragma once
#include "PostEffectBase.h"

class PostEffectChromaticAberration : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectChromaticAberration();

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

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;
};

