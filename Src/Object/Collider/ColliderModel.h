#pragma once
#include "ColliderBase.h"
#include "../Common/Transform.h"

class ColliderModel : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderModel(ActorBase& owner, const COLLISION_TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ColliderModel() override;

	/// <summary>
	/// モデルを返す
	/// </summary>
	/// <returns>モデル</returns>
	const int GetModel() const { return transformOwner_.modelId; }

private:
};

