#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/ColliderFactory.h"
#include "StageObjectBase.h"

StageObjectBase::StageObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	ActorBase(mapParam),
	STAGE_KEY(key),
	ROOM_TAG(mapParam["tag"]),
	charaMng_(CharacterManager::GetInstance())
{
	material_ = nullptr;
	renderer_ = nullptr;
	collider_ = collFtr_.Create(*this, colliderParam);
	isActive_ = true;
	isTrans_ = false;
}

StageObjectBase::~StageObjectBase()
{
}

void StageObjectBase::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("standardVs"), 2, resMng_.GetHandle("standardPs"), 3);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	// バッファーの設定
	VECTOR cameraPos = GetCameraPosition();
	VECTOR lightPos = { 0.0f,0.0f,0.0f };
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR spotLightDir = mainCamera.GetForward();

	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ lightPos.x,lightPos.y, lightPos.z, fogEnd });

	//material_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });
	//material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	//material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });

	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ lightPos.x, lightPos.y,lightPos.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });

	// 基底クラスの読み込み
	ActorBase::Load();
}

void StageObjectBase::DrawMain()
{
	// マテリアル設定
	material_->SetConstBufPS(1, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	VECTOR cameraPos = GetCameraPosition();
	VECTOR lightPos = charaMng_.GetPlayerLightPos();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR spotLightDir = mainCamera.GetForward();
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	material_->SetConstBufVS(1, FLOAT4{ lightPos.x,lightPos.y,lightPos.z, fogEnd });

	material_->SetConstBufPS(0, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->SetConstBufPS(1, FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->SetConstBufPS(2, FLOAT4{ lightPos.x, lightPos.y,lightPos.z, 0.0f });
	material_->SetConstBufPS(3, FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });
	// 描画
	renderer_->Draw();
}

void StageObjectBase::InitTransform()
{
	transform_.scl = INITIAL_SCL;
	transform_.pos = VScale(INITIAL_POS, UtilityCommon::METER_TO_UNIT_SCALE);
	transform_.quaRot = Quaternion::Euler({ UtilityCommon::Deg2RadF(INITIAL_ROT.x),UtilityCommon::Deg2RadF(INITIAL_ROT.y), UtilityCommon::Deg2RadF(INITIAL_ROT.z )});
	transform_.quaRotLocal = Quaternion::Euler({ UtilityCommon::Deg2RadF(0.0f),UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void StageObjectBase::DebugDraw()
{
	if (collider_ == nullptr)
	{
		return;
	}

	collider_->DebugDraw();
}