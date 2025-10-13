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

void Ghost::Create(const Json& param)
{
	// 読み込み処理
	Load();

	// 初期化処理
	Init();

	// トランスフォームの設定
	transform_.scl = { param["initScl"]["x"],param["initScl"]["y"],param["initScl"]["z"] };
	transform_.pos = { param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] };
	transform_.rot = { param["initRot"]["x"],param["initRot"]["y"],param["initRot"]["z"] };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
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
