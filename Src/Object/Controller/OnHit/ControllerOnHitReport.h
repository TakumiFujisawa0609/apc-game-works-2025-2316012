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
	/// <param name="owner">所有者</param>
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

	// 狂気値減少量
	static constexpr int MADNESS_DECREASE_VALUE = -100;

	// 所有者のインスタンス
	Player& owner_;

	// システム管理クラス
	GameSystemManager& systemMng_;

	// レポート
	ReportSystem* report_;

	// 衝突物別処理
	void OnHitAnomaly(const std::weak_ptr<ColliderBase>& opponentCollider);				// 異変
	void OnHitGhost(const std::weak_ptr<ColliderBase>& opponentCollider);				// ゴースト

	// 共通処理
	void OnHitCommon();
};