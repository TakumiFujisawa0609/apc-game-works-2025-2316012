#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Render/PixelMaterial.h"
#include "../../Render/PixelRenderer.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "TitleLogo.h"

TitleLogo::TitleLogo()
{
	logo_ = -1;
	step_ = 0.0f;
	isGlitch_ = false;
	timer_ = nullptr;
}

TitleLogo::~TitleLogo()
{
}

void TitleLogo::Load()
{
	int ps = resMng_.GetHandle("glitchPs");
	logo_ = resMng_.GetHandle("titleLogo");
	material_ = std::make_unique<PixelMaterial>(ps, 2);				// マテリアルの生成
	material_->AddTextureBuf(logo_);								// テクスチャの追加
	renderer_ = std::make_unique<PixelRenderer>(*material_);		// レンダラーの生成
	material_->AddConstBuf({ 0.0f, 0.0f, 0.0f, 0.0f });				// 色の初期化
	material_->AddConstBuf({ 0.0f, 0.0f, 0.0f, 0.0f });				// 色の初期化

	// タイマー
	timer_ = std::make_unique<Timer>(NORMAL_DRAW_TIME);
}

void TitleLogo::Init()
{
	isGlitch_ = false;
	pos_ = { 320, 260 };
	renderer_->SetPos(pos_);
	renderer_->SetSize({ 640, 160 });
	renderer_->MakeSquereVertex();
	timer_->InitCountUp();
}

void TitleLogo::Update()
{
	// 設定時間に達した場合
	if (timer_->CountUp())
	{
		// グリッチ中の場合
		if (isGlitch_)
		{
			// 通常描画の設定
			timer_->SetGoalTime(NORMAL_DRAW_TIME);
			isGlitch_ = false;
		}
		// 通常描画の場合
		else
		{
			// グリッチ描画設定
			timer_->SetGoalTime(GLITCH_DRAW_TIME);
			isGlitch_ = true;
		}
	}
}

void TitleLogo::Draw()
{
	if (isGlitch_)
	{
		// ステップ更新
		step_ += scnMng_.GetDeltaTime();

		// 強度
		float strength = sin(step_ * GLITCH_CYCLE_RATE) * GLITCH_AMPLITUDE_SCALE + GLITCH_OFFSET;

		// バッファー設定
		material_->SetConstBuf(1, { step_, strength, GLITCH_SPEED, DIV_NUM });

		// 描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, UtilityCommon::ALPHA_MAX);
		renderer_->Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// 通常描画
		DrawGraph(pos_.x, pos_.y, logo_, true);
	}
}
