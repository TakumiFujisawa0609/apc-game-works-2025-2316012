#include <DxLib.h>
#include "../../Manager/Resource/ResourceManager.h"
#include "../Collider/ColliderModel.h"
#include "MainStage.h"

MainStage::MainStage()
{
}

MainStage::~MainStage()
{
}

void MainStage::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle("mainStage"));

	// コライダーの生成
	collider_ = std::make_shared<ColliderModel>(*this, COLLISION_TAG::STAGE);
	MakeCollider(collider_);
}

void MainStage::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}
