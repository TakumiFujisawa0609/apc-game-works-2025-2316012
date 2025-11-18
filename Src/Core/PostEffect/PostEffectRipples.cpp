#include <cmath>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectRipples.h"

namespace
{
	// アスペクト比
	static constexpr float ASPECT_RATIO = Application::SCREEN_SIZE_X / Application::SCREEN_SIZE_Y;
}


PostEffectRipples::PostEffectRipples() : 
	input_(InputManager::GetInstance())
{
	time_ = 0.0f;
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
	material_->AddConstBuf(FLOAT4{ time_,0.0f, ASPECT_RATIO,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 基底クラスの初期化
	PostEffectBase::Init();
}

void PostEffectRipples::Draw()
{
	// ローカル変数の定義
	float wave = 0.0f;

	// エフェクトの開始条件を満たしている場合
	if (isRepples_)
	{
		// タイマーの更新
		time_ += scnMng_.GetDeltaTime();

		// 波紋距離の更新
		float wave = fmod(time_ * 0.2f, 1.5f);
	}

	// マテリアル設定
	material_->SetConstBuf(0, FLOAT4{ time_,wave, ASPECT_RATIO, 0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
