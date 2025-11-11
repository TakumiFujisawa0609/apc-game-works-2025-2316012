#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
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
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("ghostVs"), 2, resMng_.GetHandle("ghostPs"), 0);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(model_, *material_);

	// バッファーの設定
	VECTOR cameraPos = GetCameraPosition();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);

	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ fogEnd, 0.0f,0.0f,0.0f });

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
