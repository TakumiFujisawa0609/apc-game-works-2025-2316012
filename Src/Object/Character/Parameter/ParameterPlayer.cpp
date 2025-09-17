#include "ParameterPlayer.h"

ParameterPlayer::ParameterPlayer()
{
	animationJumpSpeed_ = 0.0f;
	jumpPower_ = 0.0f;
	jumpAcceptTime_ = 0.0f;
}

void ParameterPlayer::SetDataFile(const json& file)
{
	//共通データの格納
	ParameterBase::SetDataFile(file);

	//固有データの格納
	animationJumpSpeed_ = file["animationJumpSpeed"];
	jumpPower_ = file["jumpPower"];
	jumpAcceptTime_ = file["jumpAcceptTime"];
}
