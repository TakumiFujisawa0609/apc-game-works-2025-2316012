#include <DxLib.h>
#include "../../Manager/Resource/ResourceManager.h"
#include "MainStage.h"

MainStage::MainStage()
{
}

MainStage::~MainStage()
{
}

void MainStage::Load()
{
	//ƒ‚ƒfƒ‹‚Ìİ’è
	transform_.SetModel(resMng_.GetHandle("mainStage"));
}

void MainStage::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}
