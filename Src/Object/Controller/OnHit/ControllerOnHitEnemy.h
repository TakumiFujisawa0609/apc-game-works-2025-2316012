#pragma once
#include "ControllerOnHitBase.h"

class Enemy;
class ControllerActionEnemy;

class ControllerOnHitEnemy : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitEnemy(Enemy& owner);
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerOnHitEnemy() override;

private:

	// 所有者のインスタンス
	Enemy& owner_;

	// アクションクラスの参照
	ControllerActionEnemy& actionCtrl_;

	// 衝突物別処理
	void OnHitMainStage(const std::weak_ptr<ColliderBase>& opponentCollider);			// メインステージ
	void OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider);			// ステージオブジェクト
	void OnHitPlayer(const std::weak_ptr<ColliderBase>& opponentCollider);				// プレイヤー
};

