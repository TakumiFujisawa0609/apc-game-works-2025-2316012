#include "../Character/CharacterBase.h"
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
	//ˆÚ“®—Ê‚Ì¶¬
	VECTOR movePow = VScale(owner_.GetMoveDir(), owner_.GetMoveSpeed());

	//ˆÚ“®Œã‚ÌÀ•W‚ğ¶¬
	VECTOR movedPos = VAdd(owner_.GetTransform().pos, movePow);

	//À•Wİ’è
	owner_.SetPos(movedPos);
}