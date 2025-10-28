#include "StageObjectBase.h"
#include "StageGimmickObjectBase.h"
#include "Painting.h"
#include "StageMain.h"
#include "StageObjectFactory.h"

StageObjectFactory::StageObjectFactory()
{
	RegisterCreate("Painting01", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreatePainting(key, mapParam, colliderParam); });
	RegisterCreate("Painting02", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreatePainting(key, mapParam, colliderParam); });
	RegisterCreate("StageMesh", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
}

StageObjectFactory::~StageObjectFactory()
{
}

std::unique_ptr<StageObjectBase> StageObjectFactory::Create(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	// Žw’è‚Ìƒ^ƒO‚ð’Tõ
	const auto& it = createObjectMap_.find(key);

	// “o˜^‚³‚ê‚Ä‚¢‚È‚¢ê‡
	if (it == createObjectMap_.end())
	{
		// ’Êí¶¬
		return CreateStageObject(key, mapParam, colliderParam);
	}
	// ‚³‚ê‚Ä‚¢‚éê‡
	else
	{
		// Ží—Þ•Ê‚É¶¬
		return it->second(key, mapParam, colliderParam);		
	}
}

void StageObjectFactory::RegisterCreate(const std::string& type, std::function<std::unique_ptr<StageObjectBase>(const std::string&, const Json&, const Json&)> func)
{
	createObjectMap_[type] = func;
}

std::unique_ptr<StageObjectBase> StageObjectFactory::CreateStageObject(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<StageObjectBase>(key, mapParam, colliderParam);
}

std::unique_ptr<StageGimmickObjectBase> StageObjectFactory::CreateStageObjectGimmick(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<StageGimmickObjectBase>(key, mapParam, colliderParam);
}

std::unique_ptr<Painting> StageObjectFactory::CreatePainting(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<Painting>(key, mapParam, colliderParam);
}

std::unique_ptr<StageMain> StageObjectFactory::CreateStageMain(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<StageMain>(key, mapParam, colliderParam);
}
