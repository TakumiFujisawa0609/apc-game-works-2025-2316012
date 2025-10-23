#include "CollisionTags.h"

const std::map<CollisionTags::TAG, std::string> CollisionTags::TAG_NAME_MAP =
{
	{CollisionTags::TAG::NONE, "none"},
	{CollisionTags::TAG::PLAYER, "player"},
	{CollisionTags::TAG::MAIN_STAGE, "mainStage"},
	{CollisionTags::TAG::DECO_STAGE, "decoStage"},
	{CollisionTags::TAG::STAGE_GIMMICK, "stageGimmick"},
	{CollisionTags::TAG::DECO_GIMMICK, "decoGimmick"},
	{CollisionTags::TAG::REPORT, "report"},
	{CollisionTags::TAG::GHOST, "ghost"},
	{CollisionTags::TAG::ENEMY, "enemy"},
	{CollisionTags::TAG::ENEMY_VIEW, "enemyView"},
};