#include <cmath>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "PostEffectRipples.h"

PostEffectRipples::PostEffectRipples() : 
	input_(InputManager::GetInstance())
{
	ripplesStep_ = 0.0f;
	filmGrainStep_ = 0.0f;
	lineStep_ = 0.0f;
	lineGoalTime_ = 0.0f;
	isLineActive_ = false;
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
	material_->AddConstBuf(FLOAT4{ 0.0f,Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,1.0f });
	material_->AddConstBuf(FLOAT4{ 0.0f,GLITCH_STRENGTH,0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 初期化
	lineStep_ = 0.0f;
	lineGoalTime_ = LINE_INACTIVE;
	isLineActive_ = true;	// 初期時は表示する

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
	filmGrainStep_ += step;
	lineStep_ += step;

	// 線の状態を決定する
	if (lineGoalTime_ <= lineStep_)
	{
		// 初期化
		lineStep_ = 0.0f;

		// 状態の変更
		isLineActive_ = !isLineActive_;

		// 到達時間の変更
		lineGoalTime_ = isLineActive_ ? LINE_INACTIVE : LINE_ACTIVE;
	}

	// ラインの状態を数値に変換
	const float lineActive = isLineActive_ ? 1.0f : 0.0f;

	// エフェクトの開始条件を満たしている場合
	if (isRepples_)
	{
		// タイマーの更新
		ripplesStep_ += scnMng_.GetDeltaTime();

		// 波紋距離の更新
		wave = fmod(ripplesStep_ * 0.2f, 1.5f);

		// アスペクト比率
		ratio = ASPECT_RATIO;
	}

	// マテリアル設定
	material_->SetConstBuf(0, FLOAT4{ ripplesStep_,wave, ratio, 0.1f });
	material_->SetConstBuf(1, FLOAT4{ filmGrainStep_,Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, lineActive });
	material_->SetConstBuf(2, FLOAT4{ 1.0f - lineActive,GLITCH_STRENGTH, 0.0f,0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
