#include "ActorBase.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/CollisionManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Collider/ColliderBase.h"
#include "../Collider/ColliderFactory.h"

ActorBase::ActorBase(const Json& param):
	INITIAL_POS({ param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] }),
	INITIAL_ROT({ param["initRot"]["x"],param["initRot"]["y"],param["initRot"]["z"] }),
	INITIAL_SCL({ param["initScl"]["x"],param["initScl"]["y"],param["initScl"]["z"] }),
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	collMng_(CollisionManager::GetInstance()),
	collFtr_(ColliderFactory::GetInstance())
{
	isActive_ = true;	// 生成時は全員アクティブで
	isDelete_ = false;
	transform_ = {};
	collider_ = nullptr;
}

ActorBase::~ActorBase()
{
}

void ActorBase::Load()
{
	AddCollider();
}

void ActorBase::Init()
{
	InitTransform();
}

void ActorBase::Update()
{	
	if (!isActive_) { return; }

	UpdateBody();

	UpdateApply();

#ifdef _DEBUG
	DebugUpdate();
#endif
}

void ActorBase::Draw()
{
	if (!isActive_) { return; }

	DrawMain();

	DrawTranslucent();

	DrawUI();

#ifdef _DEBUG
	DebugDraw();
#endif 
}

void ActorBase::InitTransform()
{
	transform_.scl = INITIAL_SCL;
	transform_.pos = INITIAL_POS;	
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void ActorBase::UpdateBody()
{
}

void ActorBase::UpdateApply()
{
}

void ActorBase::DrawMain()
{
}

void ActorBase::DrawTranslucent()
{
}

void ActorBase::DrawUI()
{
}

void ActorBase::OnCollision()
{
}

void ActorBase::AddCollider()
{
	collMng_.Add(collider_);
}

void ActorBase::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
}

void ActorBase::DebugUpdate()
{
}

void ActorBase::DebugDraw()
{
}
