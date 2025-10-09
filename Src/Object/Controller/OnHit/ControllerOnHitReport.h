#pragma once
#include "ControllerOnHitBase.h"

class Player;
class ReportSystem;
class GameSystemManager;

class ControllerOnHitReport : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitReport(Player& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitReport() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

private:

	// 所有者のインスタンス
	Player& owner_;

	// システム管理クラス
	GameSystemManager& systemMng_;

	// レポート
	ReportSystem* report_;

	// 衝突物別処理
	void OnHitStageObject(const std::weak_ptr<ColliderBase>& opponentCollider);			// ステージオブジェクト
};