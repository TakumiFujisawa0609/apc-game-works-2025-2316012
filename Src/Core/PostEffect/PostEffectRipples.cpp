#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectRipples.h"

PostEffectRipples::PostEffectRipples(const Json& param) :
	PostEffectBase(param),
	LINE_INACTIVE(param["lineInactive"]),
	LINE_ACTIVE(param["lineActive"]),
	GLITCH_STRENGTH(param["glitchStrength"]),
	RIPPLES_WAVE_SPEED(param["ripplesWaveSpeed"]),
	RIPPLES_WAVE_MAX(param["ripplesWaveMax"]),
	FILM_GRAIN_STRENGTH(param["filmGrainStrength"]),
	TIME_FACTOR(param["timeFactor"]),
	DISTANCE_FACTOR(param["distanceFactor"]),
	TOTAL_FACTOR(param["totalFactor"]),
	WAVE_WIDTH(param["waveWidth"]),
	START_UV_X(param["startUvX"]),
	START_UV_Y(param["startUvY"]),
	BLOCK_SIZE_Y(param["blcokSizeY"])
{	
	ripplesStep_ = 0.0f;
	filmGrainStep_ = 0.0f;
	isRepples_ = false;
}

PostEffectRipples::~PostEffectRipples()
{
}

void PostEffectRipples::Init()
{
	// 描画関係の生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("repplisEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ ripplesStep_,0.0f, 0.0f,0.0f });
	material_->AddConstBuf(FLOAT4{ GLITCH_STRENGTH, TIME_FACTOR, DISTANCE_FACTOR, TOTAL_FACTOR });
	material_->AddConstBuf(FLOAT4{ WAVE_WIDTH, START_UV_X, START_UV_Y, BLOCK_SIZE_Y });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectRipples::Draw()
{
	// ローカル変数の定義
	float wave = 0.0f;
	float ratio = 0.0f;
	const float step = scnMng_.GetDeltaTime();
	constexpr float ASPECT_RATIO = Application::SCREEN_SIZE_X / Application::SCREEN_SIZE_Y;

	// 各種ステップの更新
	filmGrainStep_ += step * 0.1f;

	// エフェクトの開始条件を満たしている場合
	if (isRepples_)
	{
		filmGrainStep_ = 0.0f;

		// タイマーの更新
		ripplesStep_ += scnMng_.GetDeltaTime();

		// 波紋距離の更新
		wave = fmod(ripplesStep_ * RIPPLES_WAVE_SPEED, RIPPLES_WAVE_MAX);

		// アスペクト比率
		ratio = ASPECT_RATIO;
	}

	// マテリアル設定
	material_->SetConstBuf(0, FLOAT4{ ripplesStep_,wave, ratio, filmGrainStep_ });
	material_->SetConstBuf(1, FLOAT4{ GLITCH_STRENGTH, TIME_FACTOR, DISTANCE_FACTOR, TOTAL_FACTOR });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
