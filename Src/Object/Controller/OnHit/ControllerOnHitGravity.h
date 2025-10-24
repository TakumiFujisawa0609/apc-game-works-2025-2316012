#pragma once
#include "ControllerOnHitBase.h"

class CharacterBase;

class ControllerOnHitGravity : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitGravity(CharacterBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitGravity() override;

private:

	// 所有者のインスタンス
	CharacterBase& owner_;

	// 衝突物別処理
	void OnHitStage(const std::weak_ptr<ColliderBase>& opponentCollider);			// ステージ
};

