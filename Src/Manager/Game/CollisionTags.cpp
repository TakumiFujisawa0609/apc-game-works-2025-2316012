#include "CollisionTags.h"

const std::map<CollisionTags::TAG, std::string> CollisionTags::TAG_NAME_MAP =
{
	{CollisionTags::TAG::NONE, "none"},
	{CollisionTags::TAG::PLAYER, "player"},
	{CollisionTags::TAG::STAGE_MAIN, "stageMain"},
	{CollisionTags::TAG::STAGE_GIMMICK, "stageGimmick"},
	{CollisionTags::TAG::REPORT, "report"},
	{CollisionTags::TAG::GHOST, "ghost"},
	{CollisionTags::TAG::ENEMY, "enemy"},
	{CollisionTags::TAG::ENEMY_VIEW, "enemyView"},
	{CollisionTags::TAG::PLAYER_LIGHT, "playerLight"},
	{CollisionTags::TAG::ANOMALY, "anomaly"},
};