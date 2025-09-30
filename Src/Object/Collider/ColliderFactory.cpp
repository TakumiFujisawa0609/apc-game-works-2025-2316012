#include <algorithm> 
#include <cassert> 
#include "ColliderFactory.h"

std::shared_ptr<ColliderBase> ColliderFactory::Create(ActorBase& owner, const std::string& stringTag, const std::string& stringType)
{
	CollisionTags::TAG tag = GetTagFromStringTag(stringTag);
	return createColliderMap_[stringType](owner, tag);
}

ColliderType::TYPE ColliderFactory::GetTypeFromStringType(const std::string& type)
{
	auto & map = ColliderType::TYPE_NAME_MAP;
	auto it = std::find_if(map.begin(), map.end(),
		[&type](const auto& pair) {return pair.second == type; });
	
	// ïœä∑Ç™èoóàÇ»Ç©Ç¡ÇΩèÍçá
	assert(it == map.end() && L"ëŒâûÇµÇƒÇ¢Ç»Ç¢ÉRÉâÉCÉ_Å[ÇÃéÌóﬁÇéÛÇØéÊÇ¡ÇΩ");

	// ïœä∑ÇµÇΩå^Çï‘Ç∑
	return it->first;
}

CollisionTags::TAG ColliderFactory::GetTagFromStringTag(const std::string& tag)
{
	auto& map = CollisionTags::TAG_NAME_MAP;
	auto it = std::find_if(map.begin(), map.end(),
		[&tag](const auto& pair) {return pair.second == tag; });

	// ïœä∑Ç™èoóàÇ»Ç©Ç¡ÇΩèÍçá
	if (it == map.end())
	{
		assert(false && L"ëŒâûÇµÇƒÇ¢Ç»Ç¢è’ìÀï®ÇÃéÌóﬁÇéÛÇØéÊÇ¡ÇΩ");
		return CollisionTags::TAG::NONE;
	}

	// ïœä∑ÇµÇΩå^Çï‘Ç∑
	return it->first;
}

ColliderFactory::ColliderFactory()
{
	createColliderMap_.clear();

	// ê∂ê¨èàóùÇÃìoò^
	RegisterCreate<ColliderCapsule>(ColliderType::TYPE_NAME_MAP.at(ColliderType::TYPE::CAPSULE));
	RegisterCreate<ColliderLine>(ColliderType::TYPE_NAME_MAP.at(ColliderType::TYPE::LINE));
	RegisterCreate<ColliderModel>(ColliderType::TYPE_NAME_MAP.at(ColliderType::TYPE::MODEL));
	RegisterCreate<ColliderSphere>(ColliderType::TYPE_NAME_MAP.at(ColliderType::TYPE::SPHERE));
}

ColliderFactory::~ColliderFactory()
{
	createColliderMap_.clear();
}