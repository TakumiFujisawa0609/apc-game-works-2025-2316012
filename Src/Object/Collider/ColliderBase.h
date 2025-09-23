#pragma once

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
	ColliderBase(ActorBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderBase();	
	
	/// <summary>
	/// デバッグの描画
	/// </summary>
	virtual void DebugDraw() = 0;

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

protected:

	// 所有者
	ActorBase& owner_;

	//衝突判定
	bool isHit_;

	// 削除判定
	bool isDelete_;

	// 種類
	TYPE type_;
};

