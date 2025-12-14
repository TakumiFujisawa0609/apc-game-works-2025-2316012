#pragma once
#include "PostEffectBase.h"
#include "../Common/Vector2F.h"

class PostEffectRipples : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectRipples();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectRipples() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// エフェクト開始の設定
	/// </summary>
	void SetStart() { isRepples_ = true; };

private:

	// 線が消える時間
	static constexpr float LINE_INACTIVE = 3.0f;

	// 線を表示する時間
	static constexpr float LINE_ACTIVE = 1.0f;

	// グリッチの強さ
	static constexpr float GLITCH_STRENGTH = 10.0f;
	
	// 波紋タイマーに乗じる速度係数
	static constexpr float RIPPLES_WAVE_SPEED = 0.2f;    
	
	// 波紋が消えるまでの最大距離
	static constexpr float RIPPLES_WAVE_MAX = 1.5f;      

	// フィルムグレインの強さ
	static constexpr float FILM_GRAIN_STRENGTH = 0.1f;

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 3;

	// リップル用ステップ
	float ripplesStep_;

	// フィルムグレイン用のステップ
	float filmGrainStep_;

	// ライン用ステップ
	float lineStep_;

	// ラインの到達時間
	float lineGoalTime_;	
	
	// 波紋の開始判定
	bool isRepples_;

	// 線の活動状態
	bool isLineActive_;
};