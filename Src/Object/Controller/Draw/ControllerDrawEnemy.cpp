#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "ControllerDrawEnemy.h"

ControllerDrawEnemy::ControllerDrawEnemy(const int model) :
	ControllerDrawBase(model)
{
}

ControllerDrawEnemy::~ControllerDrawEnemy()
{
}

void ControllerDrawEnemy::Load()
{
	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("enemyVs"), 2, resMng_.GetHandle("enemyPs"), 3);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(model_, *material_);

	// バッファーの設定
	VECTOR cameraPos = GetCameraPosition();
	VECTOR lightPos = { 0.0f,0.0f,0.0f };
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ lightPos.x,lightPos.y, lightPos.z, fogEnd });

	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, 1.0f });
}

void ControllerDrawEnemy::UpdateBuffer()
{	
	VECTOR cameraPos = GetCameraPosition();	
	VECTOR lightPos = charaMng_.GetPlayerLightPos();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	// マテリアル設定
	material_->SetConstBufPS(0, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->SetConstBufPS(2, FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, 1.0f });

	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	material_->SetConstBufVS(1, FLOAT4{ lightPos.x,lightPos.y,lightPos.z, fogEnd });
}
