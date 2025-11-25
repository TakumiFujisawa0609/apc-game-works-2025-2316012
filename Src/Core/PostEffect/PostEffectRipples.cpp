#include <cmath>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectRipples.h"

PostEffectRipples::PostEffectRipples() : 
	input_(InputManager::GetInstance())
{
	time_ = 0.0f;
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
	material_->AddConstBuf(FLOAT4{ time_,0.0f, 0.0f,0.0f });
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f, 0.0f,0.0f });

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
	constexpr float ASPECT_RATIO = Application::SCREEN_SIZE_X / Application::SCREEN_SIZE_Y;

	// フィルムグレインの更新
	filmGrainStep_ += scnMng_.GetDeltaTime();

	// エフェクトの開始条件を満たしている場合
	if (isRepples_)
	{
		// タイマーの更新
		time_ += scnMng_.GetDeltaTime();

		// 波紋距離の更新
		wave = fmod(time_ * 0.2f, 1.5f);

		// アスペクト比率
		ratio = ASPECT_RATIO;
	}

	// マテリアル設定
	material_->SetConstBuf(0, FLOAT4{ time_,wave, ratio, 0.1f });
	material_->SetConstBuf(1, FLOAT4{ filmGrainStep_,0.0f, 0.0f, 0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
