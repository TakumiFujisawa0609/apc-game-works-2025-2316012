#pragma once
#include "ControllerOnHitBase.h"

class StageGimmickObjectBase;

class ControllerOnHitStageObject : public ControllerOnHitBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitStageObject(StageGimmickObjectBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerOnHitStageObject();

private:

	// 所有者の参照
	StageGimmickObjectBase& owner_;

	void OnHitReport(const std::weak_ptr<ColliderBase>& opponentCollider);				// 報告
};

