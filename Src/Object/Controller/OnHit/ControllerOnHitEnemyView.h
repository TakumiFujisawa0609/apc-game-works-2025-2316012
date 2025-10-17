#pragma once
#include "ControllerOnHitBase.h"

class Enemy;
class ControllerActionEnemy;

class ControllerOnHitEnemyView : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitEnemyView(Enemy& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerOnHitEnemyView() override;

private:

	// 所有者のインスタンス
	Enemy& owner_;

	// アクションクラスの参照
	ControllerActionEnemy& actionCtrl_;

	// 衝突物別処理
	void OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider);			// ステージオブジェクト(メインステージも含む)
};

