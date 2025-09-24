#pragma once
#include <memory>
#include <vector>
#include "../../Template/Singleton.h"

class ColliderBase;
class ColliderCapsule;
class ColliderModel;

class CollisionManager : public Singleton<CollisionManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<CollisionManager>;

public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	void Add(const std::shared_ptr<ColliderBase> collider);

	/// <summary>
	/// 削除
	/// </summary>
	void Clear();

	/// <summary>
	/// 不要のコライダーの削除
	/// </summary>
	void Sweep();

private:

	//コライダー配列
	std::vector<std::weak_ptr<ColliderBase>> colliders_;

	// カプセルとモデルの衝突判定
	bool IsHitCheckModeToCapsule(std::weak_ptr<ColliderCapsule> collCapsult, std::weak_ptr<ColliderModel> collModel);

	//コンストラクタ
	CollisionManager();

	//デストラクタ
	~CollisionManager();

};

