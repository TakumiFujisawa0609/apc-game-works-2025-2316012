#pragma once
#include "ControllerCameraBase.h"

class ControllerCameraPlayerDead : public ControllerCameraBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerCameraPlayerDead();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerCameraPlayerDead() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

