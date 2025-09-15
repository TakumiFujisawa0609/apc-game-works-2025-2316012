#include "StageManager.h"
#include "../../Object/Stage/StageObjectBase.h"
#include "../../Object/Stage/MainStage.h"

void StageManager::Load()
{
	//test用ステージオブジェクト生成
	testStage_ = std::make_unique<MainStage>();
	testStage_->Load();
}

void StageManager::Init()
{
	testStage_->Init();
}

void StageManager::Update()
{
	testStage_->Update();
}

void StageManager::Draw()
{
	testStage_->Draw();
}

StageManager::StageManager()
{
	testStage_ = nullptr;
}
