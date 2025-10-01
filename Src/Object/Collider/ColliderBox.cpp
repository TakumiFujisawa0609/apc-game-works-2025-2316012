#include<algorithm>
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../../Common/Quaternion.h"
#include "ColliderBox.h"

ColliderBox::ColliderBox(ActorBase& owner, const CollisionTags::TAG tag) : 
	ColliderBase(owner, tag)
{
	type_ = ColliderType::TYPE::BOX;
	UpdateObbAxis();
}

ColliderBox::~ColliderBox()
{
}

void ColliderBox::DebugDraw()
{
	VECTOR vertices[8];
	CalculateVertices(vertices);

	// 12本のエッジのインデックス
	constexpr int EDGS[12][2] = 
	{
		{0,1}, {0,2}, {0,4}, {1,3},
		{1,5}, {2,3}, {2,6}, {3,7},
		{4,5}, {4,6}, {5,7}, {6,7}
	};

	for (int i = 0; i < 12; ++i)
	{
		DrawLine3D(vertices[EDGS[i][0]], vertices[EDGS[i][1]], UtilityCommon::RED);
	}
}

void ColliderBox::SetHalfSize(const VECTOR& halfSize)
{
	obb_.vMin = VScale(halfSize, -1.0f);
	obb_.vMax = halfSize;
}

void ColliderBox::UpdateObbAxis(void)
{
	MATRIX rotMat;
	rotMat = transformOwner_.quaRot.ToMatrix();

	obb_.axis[0] = VTransform(VGet(1, 0, 0), rotMat); // Right
	obb_.axis[1] = VTransform(VGet(0, 1, 0), rotMat); // Up
	obb_.axis[2] = VTransform(VGet(0, 0, 1), rotMat); // Forward
}

void ColliderBox::CalculateVertices(VECTOR outVertices[8]) const
{
	MATRIX rotMat;
	rotMat = transformOwner_.quaRot.ToMatrix();

	int idx = 0;
	for (int x = 0; x <= 1; ++x)
	{
		for (int y = 0; y <= 1; ++y)
		{
			for (int z = 0; z <= 1; ++z)
			{
				VECTOR local;
				local.x = (x == 0) ? obb_.vMin.x : obb_.vMax.x;
				local.y = (y == 0) ? obb_.vMin.y : obb_.vMax.y;
				local.z = (z == 0) ? obb_.vMin.z : obb_.vMax.z;

				VECTOR world = VTransform(local, rotMat);
				world = VAdd(world, transformOwner_.pos);

				outVertices[idx++] = world;
			}
		}
	}
}

float ColliderBox::ClosestSegmentAABB(const VECTOR& segA, const VECTOR& segB, const VECTOR& aabbMin, const VECTOR& aabbMax)
{
	// 線分とAABBの最短距離を求める
	// 各軸でクランプを行う

	float t = 0.0f;
	float minDistSq = FLT_MAX;

	// 線分上の点 
	const int steps = 10;
	for (int i = 0; i <= steps; ++i)
	{
		float ft = static_cast<float>(i) / steps;
		VECTOR point = VAdd(segA, VScale(VSub(segB, segA), ft));

		// AABB内の最近接点
		VECTOR clamped = {
			std::max(aabbMin.x, std::min(point.x, aabbMax.x)),
			std::max(aabbMin.y, std::min(point.y, aabbMax.y)),
			std::max(aabbMin.z, std::min(point.z, aabbMax.z))
		};

		float distSq = Utility3D::SqrMagnitudeF(VSub(point, clamped));
		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			t = ft;
		}
	}

	return minDistSq;
}
