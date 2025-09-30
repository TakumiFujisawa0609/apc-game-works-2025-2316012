#include "ActorBase.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/CollisionManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Collider/ColliderBase.h"
#include "../Collider/ColliderFactory.h"

ActorBase::ActorBase(const Json& param):
	COLLISION_TAG(param["collisionTag"]),
	COLLIDER_TYPE(param["colliderType"]),
	INITIAL_POS({ param["initPos"]["x"],param["initPos"]["y"],param["initPos"]["z"] }),
	INITIAL_ROT({ param["initRot"]["x"],param["initRot"]["y"],param["initRot"]["z"] }),
	INITIAL_SCL({ param["initScl"]["x"],param["initScl"]["y"],param["initScl"]["z"] }),
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	collMng_(CollisionManager::GetInstance()),
	collFtr_(ColliderFactory::GetInstance())
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
	InitTransform();
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
	transform_.scl = INITIAL_SCL;
	transform_.quaRotLocal = Quaternion::Euler({ UtilityCommon::Deg2RadF(INITIAL_ROT.x), UtilityCommon::Deg2RadF(INITIAL_ROT.y),UtilityCommon::Deg2RadF(INITIAL_ROT.z) });
	transform_.pos = INITIAL_POS;
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

void ActorBase::MakeCollider(ActorBase& owner)
{	
	// 生成クラスからコライダーを取得
	collider_ = collFtr_.Create(owner, COLLISION_TAG, COLLIDER_TYPE);

	// 衝突判定管理クラスに追加
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
