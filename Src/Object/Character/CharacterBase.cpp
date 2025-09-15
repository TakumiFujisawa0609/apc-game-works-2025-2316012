#include <DxLib.h>
#include "CharacterBase.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../Common/Quaternion.h"

CharacterBase::CharacterBase()
{
}

void CharacterBase::Init()
{	
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y), 0.0f });
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.Update();
}

void CharacterBase::UpdateApply()
{
	transform_.Update();
}

void CharacterBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}
