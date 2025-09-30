#pragma once
#include "ColliderBase.h"

class ColliderSphere : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderSphere(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderSphere();

private:

};

