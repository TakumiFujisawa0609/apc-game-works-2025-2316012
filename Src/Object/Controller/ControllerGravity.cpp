#include "../../Utility/Utility3D.h"
#include "../Actor/Character/CharacterBase.h"
#include "ControllerGravity.h"

ControllerGravity::ControllerGravity(CharacterBase& owner) :
	owner_(owner),
	GRAVITY(owner_.GetGravity())
{
}

ControllerGravity::~ControllerGravity()
{
}

void ControllerGravity::Update()
{
	CalcGravityPow();
}

void ControllerGravity::CalcGravityPow()
{
	// 重力方向
	VECTOR dirGravity = Utility3D::DIR_D;

	// 重力の強さ
	float gravityPow = GRAVITY;

	// ジャンプ力を取得
	VECTOR jumpPow = owner_.GetJumpPow();

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow = VAdd(jumpPow, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, jumpPow);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow = gravity;
	}

	// 設定
	owner_.SetJumpPow(jumpPow);
}