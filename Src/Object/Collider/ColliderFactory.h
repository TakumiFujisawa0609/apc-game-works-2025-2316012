#pragma once
#include <functional>
#include <string>
#include <map>
#include "../../Template/Singleton.h"
#include "../../Manager/Generic/CollisionTags.h"
#include "ColliderCapsule.h"
#include "ColliderBase.h"
#include "ColliderLine.h"
#include "ColliderModel.h"
#include "ColliderSphere.h"
#include "ColliderType.h"

class ActorBase;
class ColliderBase;
//class ColliderCapsule;
//class ColliderLine;
//class ColliderModel;
//class ColliderSphere;

class ColliderFactory : public Singleton<ColliderFactory>
{

	friend class Singleton<ColliderFactory>;

public:

	/// <summary>
	/// 指定したコライダーを生成して返す
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	/// <param name="type">コライダー種類</param>
	/// <returns>コライダー</returns>
	std::shared_ptr<ColliderBase> Create(ActorBase& owner, const std::string& stringTag, const std::string& stringType);

private:

	// 生成処理の管理マップ
	std::map<std::string, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const CollisionTags::TAG)>> createColliderMap_;

	// コライダー生成処理の登録処理
	template <typename T>
	void RegisterCreate(const std::string& type)
	{
		createColliderMap_.emplace(type,
			[](ActorBase& owner, CollisionTags::TAG tag)
			{
				return std::make_shared<T>(owner, tag);
			});
	}

	// コライダー種類をストリング型から列挙型へ変換
	ColliderType::TYPE GetTypeFromStringType(const std::string& type);

	// タグをストリング型から列挙型へ変換
	CollisionTags::TAG GetTagFromStringTag(const std::string& tag);

	// コンストラクタ
	ColliderFactory();

	// デストラクタ
	~ColliderFactory();
};