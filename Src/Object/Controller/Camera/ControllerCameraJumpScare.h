#pragma once
#include "ControllerCameraBase.h"

class ControllerCameraJumpScare : public ControllerCameraBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerCameraJumpScare();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerCameraJumpScare() override;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};