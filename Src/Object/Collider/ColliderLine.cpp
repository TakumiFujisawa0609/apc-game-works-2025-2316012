#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "ColliderLine.h"

ColliderLine::ColliderLine(ActorBase& owner, const COLLISION_TAG tag) :
	ColliderBase(owner,tag)
{
	localPosPointHead_ = Utility3D::VECTOR_ZERO;
	localPosPointEnd_ = Utility3D::VECTOR_ZERO;
}

ColliderLine::~ColliderLine()
{
}

void ColliderLine::DebugDraw()
{
	VECTOR point1 = GetRotPos(localPosPointHead_);
	VECTOR point2 = GetRotPos(localPosPointEnd_);

	DrawLine3D(point1, point2, UtilityCommon::RED);
}
