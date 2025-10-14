#pragma once
#include "ControllerOnHitBase.h"

class Ghost;

class ControllerOnHitGhost : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	ControllerOnHitGhost(Ghost& owner);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitGhost();

private:

	// 所有者のインスタンス
	Ghost& owner_;

	// 衝突物別処理
	void OnHitRepot(const std::weak_ptr<ColliderBase>& opponentCollider);			// ステージオブジェクト
};

