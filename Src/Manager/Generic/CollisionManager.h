#pragma once
#include <memory>
#include <vector>
#include <functional>
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
	void Add(std::shared_ptr<ColliderBase> collider);

	/// <summary>
	/// 削除
	/// </summary>
	void Clear();

	/// <summary>
	/// 不要のコライダーの削除
	/// </summary>
	void Sweep();

private:

	// 衝突用タグのマトリックス
	std::vector<std::vector<bool>> collTagMatrix_;

	// 衝突判定処理用のマトリックス
	std::vector<std::vector<std::function<bool(std::weak_ptr<ColliderBase>, std::weak_ptr<ColliderBase>)>>> collFuncMatrix_;

	// コライダー配列
	std::vector<std::weak_ptr<ColliderBase>> colliders_;	
	
	// タグのマトリックスの設定設定
	void InitTagMatrix();

	// コライダー種のマトリックス設定
	void InitColliderMatrix();

	// 代入用の空の関数
	bool IsHit() { return false; }

	// カプセルとモデルの衝突判定
	bool IsHitCheckModeToCapsule(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// ラインとモデルの衝突判定
	bool IsHitCheckModeToLine(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	//コンストラクタ
	CollisionManager();

	//デストラクタ
	~CollisionManager();
};