#include "AnomalyBase.h"
#include "AnomalyAppearance.h"
#include "AnomalyBloodyRoom.h"
#include "AnomalyChairMountain.h"
#include "AnomalyGhost.h"
#include "AnomalyGrassRoom.h"
#include "AnomalyPainting.h"
#include "AnomalyFactory.h"

AnomalyFactory::AnomalyFactory()
{
    using A_TYPE = AnomalyManager::TYPE;

    // “o˜^
    RegisterCreate(A_TYPE::APPEARANCE, [this]() -> std::unique_ptr<AnomalyBase> { return CreateAppearance(); });
    RegisterCreate(A_TYPE::BLOODY_ROOM, [this]() -> std::unique_ptr<AnomalyBase> { return CreateBloodyRoom(); });
    RegisterCreate(A_TYPE::CHAIR_MOUNTAIN, [this]() -> std::unique_ptr<AnomalyBase> { return CreateChairMountain(); });
    RegisterCreate(A_TYPE::GHOST, [this]() -> std::unique_ptr<AnomalyBase> { return CreateGhost(); });
    RegisterCreate(A_TYPE::GRASS_ROOM, [this]() -> std::unique_ptr<AnomalyBase> { return CreateGrassRoom(); });
    RegisterCreate(A_TYPE::PAINTING, [this]() -> std::unique_ptr<AnomalyBase> { return CreatePainting(); });
}

AnomalyFactory::~AnomalyFactory()
{
}

std::unique_ptr<AnomalyBase> AnomalyFactory::Create(const AnomalyManager::TYPE type)
{
    return createAnomalyMap_[type]();
}

void AnomalyFactory::RegisterCreate(const AnomalyManager::TYPE type, std::function<std::unique_ptr<AnomalyBase>()> func)
{
    createAnomalyMap_[type] = func;
}

std::unique_ptr<AnomalyAppearance> AnomalyFactory::CreateAppearance()
{
    return std::make_unique<AnomalyAppearance>();
}

std::unique_ptr<AnomalyBloodyRoom> AnomalyFactory::CreateBloodyRoom()
{
    return std::make_unique<AnomalyBloodyRoom>();
}

std::unique_ptr<AnomalyChairMountain> AnomalyFactory::CreateChairMountain()
{
    return std::make_unique<AnomalyChairMountain>();
}

std::unique_ptr<AnomalyGhost> AnomalyFactory::CreateGhost()
{
    return std::make_unique<AnomalyGhost>();
}

std::unique_ptr<AnomalyGrassRoom> AnomalyFactory::CreateGrassRoom()
{
    return std::make_unique<AnomalyGrassRoom>();
}

std::unique_ptr<AnomalyPainting> AnomalyFactory::CreatePainting()
{
    return std::make_unique<AnomalyPainting>();
}
