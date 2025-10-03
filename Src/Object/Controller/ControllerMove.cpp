#include "../Actor/Character/CharacterBase.h"
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

	// ジャンプ量を加算
	movedPos = VAdd(movedPos, owner_.GetJumpPow());

	// 座標設定
	owner_.SetMovedPos(movedPos);
}

const VECTOR ControllerMove::GetMovedPos()
{
	return VECTOR();
}
