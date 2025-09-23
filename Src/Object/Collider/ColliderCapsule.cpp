#include <DxLib.h>
#include "../ActorBase.h"
#include "../Common/Transform.h"
#include "../Utility/UtilityCommon.h"
#include "ColliderCapsule.h"

ColliderCapsule::ColliderCapsule(ActorBase& owner) : 
	ColliderBase(owner),
	transformOwner_(owner_.GetTransform())
{
	radius_ = 0.0f;
	localPosTop_ = { 0.0f, 0.0f, 0.0f };
	localPosDown_ = { 0.0f, 0.0f, 0.0f };
}

ColliderCapsule::~ColliderCapsule()
{
}

void ColliderCapsule::DebugDraw()
{
	constexpr int COLOR = UtilityCommon::RED;
	constexpr int DIV_NUM = 5;

	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, DIV_NUM, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, DIV_NUM, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ğŒq‚®ü(X+)
	dir = transformOwner_.GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(X-)
	dir = transformOwner_.GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z+)
	dir = transformOwner_.GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z-)
	dir = transformOwner_.GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	constexpr float RADIUS = 10.0f;

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), DIV_NUM, RADIUS, COLOR, COLOR, true);
}

VECTOR ColliderCapsule::GetRotPos(const VECTOR& localPos) const
{
	VECTOR localRotPos = transformOwner_.quaRot.PosAxis(localPos);
	return VAdd(transformOwner_.pos, localRotPos);
}

VECTOR ColliderCapsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();
	VECTOR diff = VSub(top, down);
	constexpr float SCALE = 0.5f;
	return VAdd(down, VScale(diff, SCALE));
}