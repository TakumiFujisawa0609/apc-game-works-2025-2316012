#pragma once
#include "PostEffectBase.h"
#include "../Common/Vector2F.h"

class PostEffectRipples : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ情報</param>
	PostEffectRipples(const Json& param);

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
	const float LINE_INACTIVE;

	// 線を表示する時間
	const float LINE_ACTIVE;

	// グリッチの強さ
	const float GLITCH_STRENGTH;
	
	// 波紋タイマーに乗じる速度係数
	const float RIPPLES_WAVE_SPEED;    
	
	// 波紋が消えるまでの最大距離
	const float RIPPLES_WAVE_MAX;      

	// フィルムグレインの強さ
	const float FILM_GRAIN_STRENGTH;

	// 波の速度
	const float TIME_FACTOR;

	// 波の密度の細かさ
	const float DISTANCE_FACTOR;

	// 歪みの全体の強さ
	const float TOTAL_FACTOR;

	// 波紋の幅
	const float WAVE_WIDTH;

	// 開始位置
	const float START_UV_X;
	const float START_UV_Y;

	// ブロックの分割数
	const float BLOCK_SIZE_Y;

	// リップル用ステップ
	float ripplesStep_;

	// フィルムグレイン用のステップ
	float filmGrainStep_;
	
	// 波紋の開始判定
	bool isRepples_;
};