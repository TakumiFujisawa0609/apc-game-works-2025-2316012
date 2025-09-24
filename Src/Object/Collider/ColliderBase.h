#pragma once
#include "CollisionTag.h"
#include "../Common/Transform.h"

class ActorBase;

class ColliderBase
{
public:

	/// <summary>
	/// コライダー種類
	/// </summary>
	enum class TYPE
	{
		NONE,
		MODEL,
		SPHERE,
		CAPSULE,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderBase(ActorBase& owner, const COLLISION_TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderBase();	
	
	/// <summary>
	/// デバッグの描画
	/// </summary>
	virtual void DebugDraw() = 0;

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のインスタンス</param>
	void OnHit(std::weak_ptr<ColliderBase>& opponentCollider);

	/// <summary>
	/// 衝突してるか返す
	/// </summary>
	/// <returns>衝突していたらtrue,してない場合false</returns>
	const bool IsHit() const { return isHit_; }

	/// <summary>
	/// 削除判定
	/// </summary>
	/// <returns>削除する場合true,そうでない場合false</returns>
	const bool IsDelete() const { return isDelete_; }

	/// <summary>
	/// 自身のコライダー種類を返す
	/// </summary>
	/// <returns>種類</returns>
	const TYPE GetType() const { return type_; }

	/// <summary>
	/// 自身の衝突物の種類を返す
	/// </summary>
	/// <returns>衝突物の種類</returns>
	const COLLISION_TAG GetTag() const { return tag_; }

	/// <summary>
	/// 所有者のインスタンスを返す
	/// </summary>
	/// <returns>所有者のインスタンス</returns>
	const ActorBase& GetOwner() const { return owner_; }

protected:

	// 所有者
	ActorBase& owner_;

	// 所有者のトランスフォーム
	const Transform& transformOwner_;

	//衝突判定
	bool isHit_;

	// 削除判定
	bool isDelete_;

	// 種類
	TYPE type_;

	// 衝突物の種類
	COLLISION_TAG tag_;
};

