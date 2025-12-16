#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "ControllerDrawPainting.h"

ControllerDrawPainting::ControllerDrawPainting(const int model) :
	ControllerDrawBase(model)
{
}

ControllerDrawPainting::~ControllerDrawPainting()
{
}

void ControllerDrawPainting::Load()
{
	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("paintingVs"), BUFFER_VS_SIZE, resMng_.GetHandle("paintingPs"), BUFFER_PS_SIZE, BUFFER_MATRIX_SIZE);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(model_, *material_);

	// バッファーの設定
	VECTOR cameraPos = GetCameraPosition();
	VECTOR lightPos = { 0.0f,0.0f,0.0f };
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ fogEnd, 0.0f,0.0f,0.0f });

	material_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });

	// マトリックスバッファーの追加
	material_->AddConstBufVSMatrix(shadowMng_.GetLightViewMatrix());
	material_->AddConstBufVSMatrix(shadowMng_.GetLightProjectionMatrix());
}

void ControllerDrawPainting::UpdateBuffer()
{
	// マテリアル設定
	material_->SetConstBufPS(1, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	VECTOR cameraPos = GetCameraPosition();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
}
