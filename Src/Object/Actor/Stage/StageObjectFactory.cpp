#include "StageObjectBase.h"
#include "StageGimmickObjectBase.h"
#include "Painting.h"
#include "StageMain.h"
#include "Carpet.h"
#include "StageMesh.h"
#include "StageObjectFactory.h"
#include "GrassRoom.h"

StageObjectFactory::StageObjectFactory()
{
	RegisterCreate("Painting01", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreatePainting(key, mapParam, colliderParam); });
	RegisterCreate("Painting02", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreatePainting(key, mapParam, colliderParam); });
	RegisterCreate("StageMesh", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMesh(key, mapParam, colliderParam); });
	RegisterCreate("RoomA", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("RoomB", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("RoomC", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("RoomD", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateGrassRoom(key, mapParam, colliderParam); });
	RegisterCreate("RoomE", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("CorriderA", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("CorriderB", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("CorriderC", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("Roof", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageMain(key, mapParam, colliderParam); });
	RegisterCreate("Carpet01", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateCarpet(key, mapParam, colliderParam); });
	RegisterCreate("Carpet03", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateCarpet(key, mapParam, colliderParam); });
	RegisterCreate("Chair", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateCarpet(key, mapParam, colliderParam); });
	RegisterCreate("Carpet03", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateCarpet(key, mapParam, colliderParam); });
	RegisterCreate("StoveLit", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageObjectGimmick(key, mapParam, colliderParam); });
	RegisterCreate("Chair01", [this](const std::string& key, const Json& mapParam, const Json& colliderParam) -> std::unique_ptr<StageObjectBase> { return CreateStageObjectGimmick(key, mapParam, colliderParam); });
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

std::unique_ptr<StageMesh> StageObjectFactory::CreateStageMesh(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<StageMesh>(key, mapParam, colliderParam);
}

std::unique_ptr<StageMain> StageObjectFactory::CreateStageMain(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<StageMain>(key, mapParam, colliderParam);
}

std::unique_ptr<Carpet> StageObjectFactory::CreateCarpet(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<Carpet>(key, mapParam, colliderParam);
}

std::unique_ptr<GrassRoom> StageObjectFactory::CreateGrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam)
{
	return std::make_unique<GrassRoom>(key, mapParam, colliderParam);
}
