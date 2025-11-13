#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "ControllerDrawStage.h"

ControllerDrawStage::ControllerDrawStage(const int modelId)
	: ControllerDrawBase(modelId)
{
}

ControllerDrawStage::~ControllerDrawStage()
{
}

void ControllerDrawStage::Load()
{
	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("standardVs"), 2, resMng_.GetHandle("standardPs"), 4);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(model_, *material_);

	// カメラ位置の取得
	VECTOR cameraPos = GetCameraPosition();

	// ライト位置の取得
	VECTOR lightPos = charaMng_.GetPlayerLightPos();

	// ライト電源の取得
	float isSwitch = charaMng_.IsPlayerLight() ? 1.0f : 0.0f;

	// フォグの取得
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	// スポットライトの方向取得
	VECTOR spotLightDir = mainCamera.GetForward();

	// ライトの方向
	VECTOR lightDir = GetLightDirection();

	// PSのバッファーの追加
	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ lightPos.x,lightPos.y, lightPos.z, fogEnd });

	// VSのバッファーの更新
	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ cameraPos.x, cameraPos.y,cameraPos.z, isSwitch });
	material_->AddConstBufPS(FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });
}

void ControllerDrawStage::UpdateBuffer()
{
	// カメラ位置の取得
	VECTOR cameraPos = GetCameraPosition();

	// ライト位置の取得
	VECTOR lightPos = charaMng_.GetPlayerLightPos();

	// ライト電源の取得
	float isSwitch = charaMng_.IsPlayerLight() ? 1.0f : 0.0f;

	// フォグの取得
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	// スポットライトの方向取得
	VECTOR spotLightDir = mainCamera.GetForward();

	// ライトの方向
	VECTOR lightDir = GetLightDirection();

	// PSマテリアル設定
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	material_->SetConstBufVS(1, FLOAT4{ lightPos.x,lightPos.y,lightPos.z, fogEnd });

	// VSマテリアル設定
	material_->SetConstBufPS(0, FLOAT4{ lightDir.x, lightDir.y, lightDir.z, 0.0f });
	material_->SetConstBufPS(1, FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->SetConstBufPS(2, FLOAT4{ cameraPos.x, cameraPos.y,cameraPos.z, isSwitch });
	material_->SetConstBufPS(3, FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });
}
