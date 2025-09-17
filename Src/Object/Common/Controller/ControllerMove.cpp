#include "ControllerMove.h"

ControllerMove::ControllerMove(CharacterBase& owner) :
		owner_(owner)
{
	moveSpeed_ = 0.0f;
	dashSpeed_ = 0.0f;
	gravity_ = 0.0f;
	timeRot_ = 0.0f;
}

void ControllerMove::Update(void)
{

}