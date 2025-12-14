#include<algorithm>
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../../Common/Quaternion.h"
#include "ColliderBox.h"

ColliderBox::ColliderBox(ActorBase& owner, const CollisionTags::TAG tag) : 
	ColliderBase(owner, tag)
{
	type_ = ColliderType::TYPE::BOX;
	SetHalfSize(MV1GetScale(transformOwner_.modelId));	
	UpdateObbAxis();
}

ColliderBox::~ColliderBox()
{
}

void ColliderBox::DebugDraw()
{
	VECTOR vertices[VERTEX_COUNT];
	CalculateVertices(vertices);

	for (int i = 0; i < EDGE_COUNT; ++i)
	{
		DrawLine3D(vertices[EDGES[i][0]], vertices[EDGES[i][1]], UtilityCommon::RED);
	}
}

void ColliderBox::SetHalfSize(const VECTOR& halfSize)
{
	obb_.vMin = VScale(halfSize, -1.0f);
	obb_.vMax = halfSize;
}

std::shared_ptr<ColliderBase> ColliderBox::Clone() const
{
	return std::make_shared<ColliderBox>(*this);
}

void ColliderBox::UpdateObbAxis(void)
{
	MATRIX rotMat;
	rotMat = transformOwner_.quaRot.ToMatrix();

	obb_.axis[0] = VTransform(Utility3D::DIR_R, rotMat); // Right
	obb_.axis[1] = VTransform(Utility3D::DIR_U, rotMat); // Up
	obb_.axis[2] = VTransform(Utility3D::DIR_F, rotMat); // Forward
}

void ColliderBox::CalculateVertices(VECTOR outVertices[VERTEX_COUNT]) const
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