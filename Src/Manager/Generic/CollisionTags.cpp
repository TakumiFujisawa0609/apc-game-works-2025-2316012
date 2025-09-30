#include "CollisionTags.h"

const std::map<CollisionTags::TAG, std::string> CollisionTags::TAG_NAME_MAP =
{
	{CollisionTags::TAG::PLAYER, "player"},
	{CollisionTags::TAG::MAIN_STAGE, "mainStage"}
};