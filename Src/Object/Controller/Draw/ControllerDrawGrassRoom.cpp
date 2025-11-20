#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Actor/Stage/GrassRoom.h"
#include "ControllerDrawGrassRoom.h"

ControllerDrawGrassRoom::ControllerDrawGrassRoom(const int model, GrassRoom& owner) :
	ControllerDrawBase(model),
	owner_(owner)
{
	startPos_ = Utility3D::VECTOR_ZERO;
}

ControllerDrawGrassRoom::~ControllerDrawGrassRoom()
{
}

void ControllerDrawGrassRoom::Load()
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

	// 異変の開始位置を計算
	startPos_ = owner_.GetStartPos();

	// PSのバッファーの追加
	material_->AddConstBufVS(FLOAT4{ cameraPos.x,cameraPos.y, cameraPos.z, fogStart });
	material_->AddConstBufVS(FLOAT4{ lightPos.x,lightPos.y, lightPos.z, fogEnd });

	// VSのバッファーの更新
	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ AMBIENT.x, AMBIENT.y, AMBIENT.z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ cameraPos.x, cameraPos.y,cameraPos.z, isSwitch });
	material_->AddConstBufPS(FLOAT4{ spotLightDir.x, spotLightDir.y,spotLightDir.z,0.0f });
	material_->AddConstBufPS(FLOAT4{ startPos_.x, startPos_.y, startPos_.z,0.0f });
}

void ControllerDrawGrassRoom::Draw()
{
	ControllerDrawBase::Draw();

	DrawSphere3D(startPos_, 100.0f, 10, UtilityCommon::RED, UtilityCommon::RED, true);
}

void ControllerDrawGrassRoom::UpdateBuffer()
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
	material_->SetConstBufPS(4, FLOAT4{ startPos_.x, startPos_.y, startPos_.z,owner_.GetDistance() });
}