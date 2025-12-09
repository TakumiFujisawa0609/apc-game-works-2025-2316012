#pragma once
#include "PostEffectBase.h"

class PostEffectFilmBurn : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectFilmBurn();

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

	/// <summary>
	/// ステップの設定
	/// </summary>
	void SetStep(const float step) { step_ = step; }

private:

	// ステップ
	float step_;

	// 時間
	static constexpr float TIME = 2.0f;

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;
};

