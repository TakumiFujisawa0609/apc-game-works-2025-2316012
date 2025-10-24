#pragma once
#include "PostEffectBase.h"

class PostEffectNightView : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectNightView();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectNightView();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;

};

