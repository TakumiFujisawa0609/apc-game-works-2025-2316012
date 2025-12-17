#include "../../../../Manager/Common/SceneManager.h"
#include "../../../../Manager/Common/ResourceManager.h"
#include "../../Render/BillboardRenderer.h"
#include "../../Render/ModelMaterial.h"
#include "BillboardObjectBase.h"

BillboardObjectBase::BillboardObjectBase(const VECTOR& pos) :
	scnMng_(SceneManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	pos_(pos)
{
	isActive_ = true;
	step_ = 0.0f;
	renderer_ = nullptr;
	material_ = nullptr;
}

BillboardObjectBase::~BillboardObjectBase()
{
}

void BillboardObjectBase::Init()
{
	isActive_ = true;
	step_ = 0.0f;
}

void BillboardObjectBase::Update()
{
}

void BillboardObjectBase::Draw()
{
	// バッファー更新
	UpdateBuffer();

	// 描画
	renderer_->Draw();
}

void BillboardObjectBase::UpdateBuffer()
{
}
