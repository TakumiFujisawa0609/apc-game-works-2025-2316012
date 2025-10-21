#pragma once
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include "../../../Manager/Generic/CollisionTags.h"
#include "../ControllerBase.h"

class CharacterBase;
class ColliderBase;
class GameStateManager;

class ControllerOnHitBase : public ControllerBase
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerOnHitBase() override;

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	virtual void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() ;

protected:

	// ゲーム状態管理クラス参照
	GameStateManager& stateMng_;

	//タグ別衝突後の処理の登録
	void RegisterOnHit(const CollisionTags::TAG tag, std::function<void(const std::weak_ptr<ColliderBase>&)> onHit);

	//衝突後の処理を管理するマップ
	std::unordered_map<CollisionTags::TAG, std::function<void(const std::weak_ptr<ColliderBase>&)>> onHitMap_;

};