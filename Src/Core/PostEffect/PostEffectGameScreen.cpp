#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Game/GameSystemManager.h"
#include "../Core/Game/CameraScreen.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Object/Actor/Character/Player.h"
#include "PostEffectGameScreen.h"

PostEffectGameScreen::PostEffectGameScreen()
{
	blurStep_ = 0.0f;
	invertStep_ = 0.0f;
	flashPower_ = 0.0f;
}

PostEffectGameScreen::~PostEffectGameScreen()
{
}

void PostEffectGameScreen::Init()
{
	// 生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("gameScreenEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// バッファー追加
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,0.0f });
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 親クラスの初期化処理
	PostEffectBase::Init();

	// 変数の初期化
	blurStep_ = 0.0f;
	invertStep_ = 1.0f;
	flashPower_ = 0.0f;
}

void PostEffectGameScreen::Draw()
{
	// プレイヤーの狂気値を取得
	auto* player = dynamic_cast<Player*>(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER));
	int madnessValue = player->GetMadnessValue();

	// シロクロの判定
	auto* cameraScreen = dynamic_cast<CameraScreen*>(&GameSystemManager::GetInstance().GetGamsSystem(GameSystemManager::TYPE::SCREEN));
	float isGray_ = cameraScreen->GetScreenMode() == CameraScreen::SCREEN_MODE::NORMAL ? 0.0f : 1.0f;

	// プレイヤーの狂気値が一定未満の場合
	if (madnessValue < BLUR_CONDITION)
	{
		// 初期化
		blurStep_ = 0.0f;
		flashPower_ = 0.0f;
	}
	else
	{
		// ブラーの制限値を決める
		const float blurLimit = static_cast<float>(MAX_BLUR_DISTANCE / BLUR_CONDITION) * (madnessValue - BLUR_CONDITION);

		// ブラーの距離を更新
		blurStep_ += BLUR_SPEED * invertStep_;

		// 条件値に達したら反転
		if (blurStep_ >= blurLimit ||
			blurStep_ <= 0.0f)
		{
			invertStep_ *= -1.0f;
		}

		// フラッシュの強度を決める
		flashPower_ = MAX_FLASH_POWER / BLUR_CONDITION * (madnessValue - BLUR_CONDITION);
	}

	// バッファーの更新
	material_->SetConstBuf(0, FLOAT4{ blurStep_, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, flashPower_ });
	material_->SetConstBuf(1, FLOAT4{ isGray_,0.0f, 0.0f,0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
