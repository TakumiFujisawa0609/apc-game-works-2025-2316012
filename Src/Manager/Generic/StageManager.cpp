#include "../../Object/Actor/Stage/StageObjectBase.h"
#include "../../Object/System/Load/ParameterLoad.h"
#include "../../Utility/UtilityLoad.h"
#include "StageManager.h"

void StageManager::Load()
{
	// パラメータステージマップを取得
	auto& paramStageMap = UtilityLoad::GetJsonData(FILE_NAME);

	// パラメーターコライダーマップを取得
	auto& paramColliderMap = UtilityLoad::GetJsonData("StageObjectsCollider");

	// パラメータ数分オブジェクト生成
	for (auto& params : paramStageMap)
	{		
		// オブジェクト格納用配列
		std::vector<std::unique_ptr<StageObjectBase>> objects;

		// 要素分パラメータ格納
		for (auto& param : params.second)
		{
			// コライダー情報を検索
			const auto& collInfos = paramColliderMap.find(params.first);

			// 中身が空もしくは配列で格納されている場合
			if (collInfos->second.empty() || collInfos->second.size() > 1)
			{
				// 想定外のため終了
				return;
			}

			// オブジェクト生成
			auto object = std::make_unique<StageObjectBase>(params.first, param, collInfos->second[0]);

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

StageManager::~StageManager()
{
}
