#include <DxLib.h>
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../Collider/ColliderModel.h"
#include "MainStage.h"

MainStage::MainStage(const std::string& key, const Json& param) :
	StageObjectBase(key, param)
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
	MakeCollider(*this);
}

void MainStage::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}
