#include "ParameterBase.h"

ParameterBase::ParameterBase()
{
	moveSpeed_ = 0.0f;
	dashSpeed_ = 0.0f;
	gravity_ = 0.0f;
	timeRot_ = 0.0f;
	initialPosition_ = VGet(0.0f, 0.0f, 0.0f);
	animationDefaultSpeed_ = 0.0f;
}

void ParameterBase::SetDataFile(const json& file)
{
	moveSpeed_ = file["moveSpeed"];
	dashSpeed_ = file["dashSpeed"];
	gravity_ = file["gravity"];
	timeRot_ = file["timeRot"];
	initialPosition_.x = file["initialPosition"]["x"];
	initialPosition_.y = file["initialPosition"]["y"];
	initialPosition_.z = file["initialPosition"]["z"];
	animationDefaultSpeed_ = file["animationDefaultSpeed"];
}
