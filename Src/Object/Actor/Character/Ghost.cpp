#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Controller/Action/ControllerActionBase.h"
#include "../../Controller/ControllerMove.h"
#include "../../Controller/ControllerRotate.h"
#include "../../Controller/ControllerGravity.h"
#include "../../Controller/OnHit/ControllerOnHitBase.h"
#include "../../Collider/ColliderCapsule.h"
#include "Ghost.h"

Ghost::Ghost(const Json& param) :
	CharacterBase(param)
{
}

Ghost::~Ghost()
{
}

void Ghost::Load()
{
	CharacterBase::Load();
}

void Ghost::Init()
{
	CharacterBase::Init();
}

void Ghost::DrawMain()
{
}

void Ghost::InitAnimation()
{
}

void Ghost::InitTransform()
{
}
