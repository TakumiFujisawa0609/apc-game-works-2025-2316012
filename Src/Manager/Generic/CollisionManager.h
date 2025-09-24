#pragma once
#include <memory>
#include <vector>
#include "../../Template/Singleton.h"

class ColliderBase;

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
	std::vector<std::shared_ptr<ColliderBase>>colliders_;

	//コンストラクタ
	CollisionManager();

	//デストラクタ
	~CollisionManager();

};

