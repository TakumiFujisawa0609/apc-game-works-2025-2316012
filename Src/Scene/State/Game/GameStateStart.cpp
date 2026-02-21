#include "../Application.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Game/GameStateManager.h"
#include "../Manager/Game/ShadowManager.h"
#include "../Manager/Game/StageManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Core/Common/Timer.h"
#include "GameStateStart.h"

GameStateStart::GameStateStart()
{
	fadeStep_ = 0.0f;
}

GameStateStart::~GameStateStart()
{
}

void GameStateStart::Init()
{
	ResourceManager& resMng = ResourceManager::GetInstance();

	// マテリアル
	material_ = std::make_unique<PixelMaterial>(resMng.GetHandle("gameStartShader"), 1);

	// レンダラー
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// タイマー
	timer_ = std::make_unique<Timer>(WAIT_TIME);
	timer_->InitCountUp();

	// テクスチャの設定
	material_->AddTextureBuf(resMng.GetHandle("gameStart"));

	// 定数バッファーの設定
	material_->AddConstBuf(FLOAT4{0.0f, TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 0.0f });

	// レンダラー設定
	renderer_->SetPos({ Application::SCREEN_HALF_X - TEXTURE_SIZE_X / 2, Application::SCREEN_HALF_Y - TEXTURE_SIZE_Y / 2 });
	renderer_->SetSize({ TEXTURE_SIZE_X, TEXTURE_SIZE_Y});
	renderer_->MakeSquereVertex();

	// 更新処理の変更
	update_ = std::bind(&GameStateStart::UpdateWait, this);
}

void GameStateStart::Update()
{
	update_();
}

void GameStateStart::Draw()
{
	// 影
	shadowMng_.Draw();

	// ステージ
	stageMng_.Draw();

	// メッセージの描画
	material_->SetConstBuf(0, FLOAT4{ fadeStep_, TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 0.0f });
	renderer_->Draw();
}

void GameStateStart::UpdateWait()
{
	// タイマーの更新
	if (timer_->CountUp())
	{
		// タイマーの時間変更
		timer_->SetGoalTime(GAME_START_TIME);
		timer_->InitCountUp();

		// 更新処理の変更
		update_ = std::bind(&GameStateStart::UpdateEffect, this);

		// 初期の効果音を再生
		SoundManager::GetInstance().PlaySe(SoundType::SE::FIRE_BURN);
	}
}

void GameStateStart::UpdateEffect()
{
	fadeStep_ += SceneManager::GetInstance().GetDeltaTime() * FADE_COEFFICIENT;

	// タイマーの更新
	if (timer_->CountUp())
	{
		// 効果音のフェードアウト
		SoundManager::GetInstance().FadeOutSe(SoundType::SE::FIRE_BURN);

		// カメラをFPSに変更
		mainCamera.ChangeMode(Camera::MODE::FPS);

		// 状態遷移
		GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::PLAY);
		return;
	}
}