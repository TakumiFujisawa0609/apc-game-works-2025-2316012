#include "../../Collider/ColliderBox.h"
#include "Room.h"

Room::Room(const std::string& key, const Json& mapParam, const Json& colliderParam) :
	StageObjectBase(key, mapParam, colliderParam)
{
}

Room::~Room()
{
}

void Room::Init()
{
	StageObjectBase::Init();
}

bool Room::CheckCameraViewClip()
{
	// Box‚Ö•ÏŠ·
	const auto& colliderBox = std::dynamic_pointer_cast<ColliderBox>(collider_);

	// ƒJƒƒ‰ŠO‚©”»’è
	if (CheckCameraViewClip_Box(colliderBox->GetVecMax(), colliderBox->GetVecMin()))
	{
		// Ž‹ŠE‚É“ü‚Á‚Ä‚È‚¢
		return false;
	}
	// Ž‹ŠE‚É“ü‚Á‚Ä‚¢‚é
	return true;
}
