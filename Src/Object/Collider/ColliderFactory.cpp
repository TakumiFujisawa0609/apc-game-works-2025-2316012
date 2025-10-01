#include <algorithm> 
#include <cassert> 
#include "ColliderBase.h"
#include "ColliderCapsule.h"
#include "ColliderLine.h"
#include "ColliderSphere.h"
#include "ColliderModel.h"
#include "ColliderBox.h"
#include "ColliderFactory.h"

std::shared_ptr<ColliderBase> ColliderFactory::Create(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	return createColliderMap_[info.type](owner, info);
}

void ColliderFactory::RegisterCreate(const std::string& type, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const ColliderInfo&)> func)
{
	createColliderMap_[type] = func;
}

std::shared_ptr<ColliderCapsule> ColliderFactory::CreateCapsule(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	auto capsule = std::make_shared<ColliderCapsule>(owner, tag);
	capsule->SetLocalPosTop(info.headPos);
	capsule->SetLocalPosDown(info.endPos);
	capsule->SetRadius(info.radius);
	return capsule;
}

std::shared_ptr<ColliderLine> ColliderFactory::CreateLine(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	auto line = std::make_shared<ColliderLine>(owner, tag);
	line->SetLocalPosPointHead(info.headPos);
	line->SetLocalPosPointEnd(info.endPos);
	return line;
}

std::shared_ptr<ColliderModel> ColliderFactory::CreateModel(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	auto model = std::make_shared<ColliderModel>(owner, tag);
	return model;
}

std::shared_ptr<ColliderSphere> ColliderFactory::CreateSphere(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	auto sphere = std::make_shared<ColliderSphere>(owner, tag);
	sphere->SetRadius(info.radius);
	return sphere;
}

std::shared_ptr<ColliderBox> ColliderFactory::CreateBox(ActorBase& owner, const ColliderInfo& info)
{
	CollisionTags::TAG tag = GetTagFromStringTag(info.tag);
	auto box = std::make_shared<ColliderBox>(owner, tag);
	return box;
}

//ColliderType::TYPE ColliderFactory::GetTypeFromStringType(const std::string& type)
//{
//	auto & map = ColliderType::TYPE_NAME_MAP;
//	auto it = std::find_if(map.begin(), map.end(),
//		[&type](const auto& pair) {return pair.second == type; });
//	
//	// •ÏŠ·‚ªo—ˆ‚È‚©‚Á‚½ê‡
//	assert(it == map.end() && L"‘Î‰ž‚µ‚Ä‚¢‚È‚¢ƒRƒ‰ƒCƒ_[‚ÌŽí—Þ‚ðŽó‚¯Žæ‚Á‚½");
//
//	// •ÏŠ·‚µ‚½Œ^‚ð•Ô‚·
//	return it->first;
//}

CollisionTags::TAG ColliderFactory::GetTagFromStringTag(const std::string& tag)
{
	auto& map = CollisionTags::TAG_NAME_MAP;
	auto it = std::find_if(map.begin(), map.end(),
		[&tag](const auto& pair) {return pair.second == tag; });

	// •ÏŠ·‚ªo—ˆ‚È‚©‚Á‚½ê‡
	if (it == map.end())
	{
		assert(false && L"‘Î‰ž‚µ‚Ä‚¢‚È‚¢Õ“Ë•¨‚ÌŽí—Þ‚ðŽó‚¯Žæ‚Á‚½");
		return CollisionTags::TAG::NONE;
	}

	// •ÏŠ·‚µ‚½Œ^‚ð•Ô‚·
	return it->first;
}

ColliderFactory::ColliderFactory()
{
	createColliderMap_.clear();

	// ¶¬ˆ—‚Ì“o˜^
	using C_TYPE = ColliderType::TYPE;
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::CAPSULE), [this](ActorBase& owner, const ColliderInfo& info) -> std::shared_ptr<ColliderBase> { return CreateCapsule(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::LINE), [this](ActorBase& owner, const ColliderInfo& info) -> std::shared_ptr<ColliderBase> { return CreateLine(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::MODEL), [this](ActorBase& owner, const ColliderInfo& info)  -> std::shared_ptr<ColliderBase> { return CreateModel(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::SPHERE), [this](ActorBase& owner, const ColliderInfo& info) -> std::shared_ptr<ColliderBase> { return CreateSphere(owner, info); });
	RegisterCreate(ColliderType::TYPE_NAME_MAP.at(C_TYPE::BOX), [this](ActorBase& owner, const ColliderInfo& info) -> std::shared_ptr<ColliderBase> { return CreateBox(owner, info); });
}

ColliderFactory::~ColliderFactory()
{
	createColliderMap_.clear();
}