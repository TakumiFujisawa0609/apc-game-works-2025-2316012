#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../../Collider/ColliderBox.h"
#include "../../Collider/ColliderModel.h"
#include "StageMain.h"

StageMain::StageMain(const std::string& key, const Json& mapParam, const Json& colliderParam):
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
	isActive_ = false;
}

StageMain::~StageMain()
{
}

void StageMain::Load()
{
	//// モデルの設定
	//transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	//// 衝突後処理の生成
	//onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);

	//// マテリアル生成
	//material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("stageMainVs"), 2, resMng_.GetHandle("stageMainPs"), 3);

	//// レンダラー生成
	//renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	//// バッファーの設定
	//VECTOR cameraPos = GetCameraPosition();
	//VECTOR lightPos = { 0.0f,0.0f,0.0f };
	//float fogStart;
	//float fogEnd;
	//GetFogStartEnd(&fogStart, &fogEnd);

	//material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	//material_->AddConstBufVS(FLOAT4{ lightPos.x,lightPos.y, lightPos.z, fogEnd });
	//material_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });
	//material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	//material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });

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

	//material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, AMBIENT.x });
	//material_->AddConstBufPS(FLOAT4{ AMBIENT.y, AMBIENT.z, lightPos.x, lightPos.y });
	//material_->AddConstBufPS(FLOAT4{ lightPos.z, spotLightDir.x,spotLightDir.y,spotLightDir.z });

	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ lightPos.x, lightPos.y,lightPos.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });

	// 基底クラスの読み込み
	ActorBase::Load();
}

void StageMain::Init()
{
	// 基底クラスの初期化処理
	StageGimmickObjectBase::Init();
}

void StageMain::Draw()
{
	// 本オブジェクトは活動状態に限らず描画を行う
	DrawMain();
}

void StageMain::SetAnomaly()
{
	// テクスチャを追加
	material_->SetTextureBuf(3, resMng_.GetHandle("bloodyHands"));

	// 報告用にコライダーを設定
	colliderModel_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::DECO_GIMMICK);
	collMng_.Add(colliderModel_);

	// 活動状態の変更
	isActive_ = true;
}

void StageMain::Refresh()
{
	// テクスチャを追加
	material_->SetTextureBuf(3, -1);

	// コライダー削除
	colliderModel_->SetDelete();
	colliderModel_ = nullptr;

	// 活動状態の変更
	isActive_ = false;
}

bool StageMain::CheckCameraViewClip()
{
	// Boxへ変換
	const auto& colliderBox = std::dynamic_pointer_cast<ColliderBox>(collider_);

	// カメラ外か判定
	if (CheckCameraViewClip_Box(colliderBox->GetVecMax(), colliderBox->GetVecMin()))
	{
		// 視界に入ってない
		return false;
	}
	// 視界に入っている
	return true;
}

void StageMain::DrawMain()
{
	// 通常描画
	VECTOR cameraPos = GetCameraPosition();
	VECTOR lightPos = charaMng_.GetPlayerLightPos();
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR spotLightDir = mainCamera.GetForward();
	material_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z, fogStart });
	material_->SetConstBufVS(1, FLOAT4{ lightPos.x,lightPos.y,lightPos.z, fogEnd });

	material_->SetConstBufPS(0,FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->SetConstBufPS(1,FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->SetConstBufPS(2,FLOAT4{ lightPos.x, lightPos.y,lightPos.z, 0.0f });
	material_->SetConstBufPS(3,FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });
	// 描画
	renderer_->Draw();
}
