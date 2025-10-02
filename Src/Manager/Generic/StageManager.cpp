#include "../../Object/Actor/Stage/StageObjectBase.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "../../Utility/UtilityLoad.h"
#include "StageManager.h"

void StageManager::Load()
{
	// パラメータマップを取得
	auto& paramMap = UtilityLoad::GetJsonData(FILE_NAME);

	// パラメータ数分オブジェクト生成
	for (auto& params : paramMap)
	{		
		// オブジェクト格納用配列
		std::vector<std::unique_ptr<StageObjectBase>> objects;

		// 要素分パラメータ格納
		for (auto& param : params.second)
		{
			// オブジェクト生成
			auto object = std::make_unique<StageObjectBase>(params.first, param);

			// オブジェクト読み込み
			object->Load();

			// 配列に格納
			objects.push_back(std::move(object));
		}
		
		// マップに登録
		stageObjectsMap_.emplace(params.first, std::move(objects));
	}
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