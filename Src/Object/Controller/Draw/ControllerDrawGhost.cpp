#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "ControllerDrawGhost.h"

ControllerDrawGhost::ControllerDrawGhost(const int model) : 
	ControllerDrawBase(model)
{
}

ControllerDrawGhost::~ControllerDrawGhost()
{
}

void ControllerDrawGhost::Load()
{
	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("standardVs"), 2, resMng_.GetHandle("grassRoomPs"), 5);

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

void ControllerDrawGhost::UpdateBuffer()
{
	VECTOR cameraPos = GetCameraPosition();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	material_->SetConstBufVS(1, FLOAT4{ fogEnd, 0.0f,0.0f, 0.0f });
}
