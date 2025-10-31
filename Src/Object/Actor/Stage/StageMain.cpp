#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../Collider/ColliderBox.h"
#include "StageMain.h"

StageMain::StageMain(const std::string& key, const Json& mapParam, const Json& colliderParam):
	StageObjectBase(key, mapParam, colliderParam)
{
	renderer_ = nullptr;
	material_ = nullptr;
}

StageMain::~StageMain()
{
}

void StageMain::Load()
{
	// 基底クラスの読み込み処理
	StageObjectBase::Load();

	// マテリアル生成
	material_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("standardVS"), 0, resMng_.GetHandle("standardPS"), 3);

	// レンダラー生成
	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	// バッファーの設定
	material_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	material_->AddConstBufPS(FLOAT4{ 0.01f, 0.01f, 0.01f, 0.0f });
}

void StageMain::Init()
{
	// 基底クラスの初期化処理
	StageObjectBase::Init();
}

void StageMain::SetAnomaly()
{
	// テクスチャを追加
	material_->SetTextureBuf(3, resMng_.GetHandle("bloodyHands"));
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
	//MV1DrawModel(transform_.modelId);
	
	// マテリアル設定
	material_->SetConstBufPS(1, FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });
	
	// 描画
	renderer_->Draw();
}
