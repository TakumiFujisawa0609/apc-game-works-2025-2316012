#include "../Character/CharacterBase.h"
#include "../Collider/ColliderLine.h"
#include "../../Utility/Utility3D.h"
#include "ControllerMove.h"

ControllerMove::ControllerMove(CharacterBase& owner) :
		owner_(owner)
{
}

ControllerMove::~ControllerMove()
{
}

void ControllerMove::Init()
{
}

void ControllerMove::Update()
{
	// 移動量の生成
	VECTOR movePow = VScale(owner_.GetMoveDir(), owner_.GetMoveSpeed());

	// 移動後の座標を生成
	VECTOR movedPos = VAdd(owner_.GetTransform().pos, movePow);

	// 座標設定
	owner_.SetPos(movedPos);

	// 重力衝突用の設定
	const auto& coll = owner_.GetGravityCollider();
	float checkPow = 10.0f;
	VECTOR headPos = VAdd(movedPos, VScale(Utility3D::DIR_U, owner_.GetGravity()));
	coll->SetLocalPosPointHead(VAdd(headPos, VScale(Utility3D::DIR_U, checkPow * 2.0f)));
	coll->SetLocalPosPointEnd(VAdd(movedPos, VScale(Utility3D::DIR_D, checkPow)));
}