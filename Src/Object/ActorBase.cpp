#include "ActorBase.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/CollisionManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "Collider/ColliderBase.h"

ActorBase::ActorBase():
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	collMng_(CollisionManager::GetInstance())
{
	transform_ = {};
	collider_ = nullptr;
}

ActorBase::~ActorBase()
{
}

void ActorBase::Load()
{
}

void ActorBase::Init()
{
}

void ActorBase::Update()
{
	UpdateMain();

	UpdateApply();

#ifdef _DEBUG
	DebugUpdate();
#endif
}

void ActorBase::Draw()
{
	DrawMain();

	DrawTranslucent();

	DrawUI();

#ifdef _DEBUG
	DebugDraw();
#endif 
}

void ActorBase::InitTransform()
{
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y), 0.0f });
	transform_.pos = Utility3D::VECTOR_ZERO;
	transform_.Update();
}

void ActorBase::UpdateMain()
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

void ActorBase::MakeCollider(std::shared_ptr<ColliderBase> collider)
{
	collMng_.Add(collider);
}

void ActorBase::DebugUpdate()
{
}

void ActorBase::DebugDraw()
{
}
