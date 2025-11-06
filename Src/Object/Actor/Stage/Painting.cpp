#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "Painting.h"

Painting::Painting(const std::string& key, const Json& mapParam, const Json& colliderParam) : 
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
	isActive_ = false;
}

Painting::~Painting()
{
}

void Painting::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("paintingVs"), 2, resMng_.GetHandle("paintingPs"), 3);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

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

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);

	// 基底クラスの読み込み
	ActorBase::Load();
}

void Painting::Draw()
{
	// 本オブジェクトは活動状態に限らず描画を行う
	DrawMain();
}

void Painting::Refresh()
{
	// テクスチャを戻す
	MV1SetTextureGraphHandle(transform_.modelId, textureIndex_, preTexture_, true);

	// 初期化
	textureIndex_ = -1;
	preTexture_ = -1;

	isActive_ = false;
}

void Painting::DrawMain()
{
	// マテリアル設定
	material_->SetConstBufPS(1, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	VECTOR cameraPos = GetCameraPosition();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	// 描画
	renderer_->Draw();
}
