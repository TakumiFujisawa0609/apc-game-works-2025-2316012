#include "ControllerGravity.h"

ControllerGravity::ControllerGravity(const float gravityPow, const VECTOR gravityDir) :
	gravity_(gravityPow),
	dir_(gravityDir)
{
}

void ControllerGravity::CalcGravityPow(VECTOR& jumpPow)
{
	// 重力
	VECTOR gravity = VScale(dir_, gravity_);
	jumpPow = VAdd(jumpPow, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dir_, jumpPow);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow = gravity;
	}
}