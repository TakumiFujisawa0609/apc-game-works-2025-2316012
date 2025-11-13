#pragma once
#include "PostEffectBase.h"

class PostEffectSnowNoise : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectSnowNoise();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectSnowNoise() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

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
	static constexpr int CONST_BUFFER_SIZE = 2;

	// 時間
	float time_;
};

