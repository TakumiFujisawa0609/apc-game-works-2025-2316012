#include "../../Object/Actor/Stage/StageObjectBase.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "StageManager.h"

void StageManager::Load()
{
	// パラメータ読み込み
	paramLoad_ = std::make_unique<ParameterLoad>(FILEN_NAME, NAME_LIST);
	paramLoad_->Load();

	// オブジェクト生成
	auto object = std::make_unique<StageObjectBase>(NAME_LIST[0], paramLoad_->GetParameterFile(NAME_LIST[0]));

	// オブジェクト読み込み
	object->Load();

	std::vector<std::unique_ptr<StageObjectBase>> objects;
	objects.push_back(std::move(object));

	// マップに登録
	stageObjectsMap_.emplace(NAME_LIST[0], std::move(objects));
}

void StageManager::Init()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			object->Init();
		}
	}
}

void StageManager::Update()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			object->Init();
		}
	}
}

void StageManager::Draw()
{
	for (auto& objects : stageObjectsMap_)
	{
		for (auto& object : objects.second)
		{
			object->Draw();
		}
	}
}

StageManager::StageManager()
{
}