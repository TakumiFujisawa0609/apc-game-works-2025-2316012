#pragma once
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include "../ControllerBase.h"

class CharacterBase;
class ColliderBase;
enum class COLLISION_TAG;

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
	virtual void OnHit(std::weak_ptr<ColliderBase>& opponentCollider);

protected:

	//タグ別衝突後の処理の登録
	void RegisterOnHit(const COLLISION_TAG tag, std::function<void(std::weak_ptr<ColliderBase>&)> onHit);

	//衝突後の処理を管理するマップ
	std::unordered_map<COLLISION_TAG, std::function<void(std::weak_ptr<ColliderBase>&)>> onHitMap_;

};

