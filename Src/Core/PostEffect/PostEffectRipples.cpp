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
	amplitude_ = 0.0f;
	frequency_ = 0.0f;
}

PostEffectRipples::~PostEffectRipples()
{
}

void PostEffectRipples::Init()
{
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("repplisEffect"), CONST_BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// パラメーター
	float Amplitude = 0.03f;
	float Frequency = 40.0f;
	float Speed = 5.0f;
	Vector2F CenterUV = { 0.5f, 0.5f };

	// マテリアル設定
	material_->AddConstBuf(FLOAT4{ time_,Amplitude,Frequency,CenterUV.x });
	material_->AddConstBuf(FLOAT4{ CenterUV.y,0.5f, -0.5f, -1.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// 基底クラスの処理
	PostEffectBase::Init();
}

void PostEffectRipples::Draw()
{
	float Speed = 5.0f;
	Vector2F pos = {};

	// 入力処理をした場合
	if (input_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		time_ = 0.0f;
		amplitude_ = 0.01f;
		frequency_ = 20.0f;

		// マウス入力を検知した場合
		if (GetMouseInput() == MOUSE_INPUT_LEFT)
		{
			// マウス位置
			Vector2F pos = input_.GetMousePos().ToVector2F();

			// 正規化
			pos = { UtilityCommon::NormalizeMinMax(pos.x, 0, Application::SCREEN_SIZE_X), UtilityCommon::NormalizeMinMax(pos.y, 0.0f, Application::SCREEN_SIZE_Y) };
		}
	}
	//else if (time_ > 3.0f);
	//{
	//	time_ = 0.0f;
	//	amplitude_ = 0.0f;
	//	frequency_ = 0.0f;
	//}

	// タイマーの更新
	time_ += scnMng_.GetDeltaTime();

	// マテリアル設定
	material_->SetConstBuf(0, FLOAT4{ time_,amplitude_,frequency_, 0.5f });
	material_->SetConstBuf(1, FLOAT4{ 0.5f ,0.8f, -0.5f, -0.7f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}
