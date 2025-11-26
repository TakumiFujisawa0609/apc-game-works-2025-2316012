#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "ControllerDrawBase.h"

ControllerDrawBase::ControllerDrawBase(const int model) :
	charaMng_(CharacterManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	model_(model)
{
	material_ = nullptr;
	renderer_ = nullptr;
}

ControllerDrawBase::~ControllerDrawBase()
{
}

void ControllerDrawBase::Load()
{
}

void ControllerDrawBase::Draw()
{
	// マテリアルの中身が空の場合
	if (material_ == nullptr)
	{
		// 通常描画
		MV1DrawModel(model_);
		return;
	}

	// バッファーの更新
	UpdateBuffer();

	// 描画
	renderer_->Draw();
}

void ControllerDrawBase::SetTexture(const int texture)
{
	// テクスチャを追加
	material_->SetTextureBuf(7, texture);
}

void ControllerDrawBase::UpdateBuffer()
{
}
