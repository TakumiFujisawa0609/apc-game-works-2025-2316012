#pragma once
#include <DxLib.h>
#include "ControllerBase.h"

class ControllerGravity : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="gravityPow">重力</param>
	/// <param name="gravityDir">重力方向</param>
	ControllerGravity(const float gravityPow, const VECTOR gravityDir);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerGravity() = default;

	/// <summary>
	/// 重力の影響を加える
	/// </summary>
	/// <param name="jumpPow">ジャンプ力</param>
	void CalcGravityPow(VECTOR& jumpPow);

private:	
	
	//重力
	float gravity_;

	//重力方向
	VECTOR dir_;
};