#pragma once
#include <DxLib.h>
#include <functional>
#include <string>
#include <map>
#include "../../Template/Singleton.h"
#include "../../Manager/Generic/CollisionTags.h"

class ActorBase;
class ColliderBase;
class ColliderCapsule;
class ColliderLine;
class ColliderModel;
class ColliderSphere;
class ColliderBox;

class ColliderFactory : public Singleton<ColliderFactory>
{

	friend class Singleton<ColliderFactory>;

public:

	/// <summary>
	/// コライダーの情報
	/// </summary>
	struct ColliderInfo
	{
		std::string tag = "";	// タグ
		std::string type = "";	// 種類
		float radius = 0.0f;	// 半径
		VECTOR headPos = {};	// 頭部座標
		VECTOR endPos = {};		// 末尾座標
	};

	/// <summary>
	/// 指定したコライダーを生成して返す
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	/// <param name="type">コライダー種類</param>
	/// <returns>コライダー</returns>
	std::shared_ptr<ColliderBase> Create(ActorBase& owner, const ColliderInfo& info);

private:

	// 生成処理の管理マップ
	std::map<std::string, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const ColliderInfo&)>> createColliderMap_;

	// コライダー生成処理の登録処理
	void RegisterCreate(const std::string& type, std::function<std::shared_ptr<ColliderBase>(ActorBase&, const ColliderInfo&)> func);

	// 各種コライダーの生成
	std::shared_ptr<ColliderCapsule> CreateCapsule(ActorBase& owner, const ColliderInfo& info);
	std::shared_ptr<ColliderLine> CreateLine(ActorBase& owner, const ColliderInfo& info);
	std::shared_ptr<ColliderModel> CreateModel(ActorBase& owner, const ColliderInfo& info);
	std::shared_ptr<ColliderSphere> CreateSphere(ActorBase& owner, const ColliderInfo& info);
	std::shared_ptr<ColliderBox> CreateBox(ActorBase& owner, const ColliderInfo& info);

	// コライダー種類をストリング型から列挙型へ変換
	//ColliderType::TYPE GetTypeFromStringType(const std::string& type);

	// タグをストリング型から列挙型へ変換
	CollisionTags::TAG GetTagFromStringTag(const std::string& tag);

	// コンストラクタ
	ColliderFactory();

	// デストラクタ
	~ColliderFactory();
};