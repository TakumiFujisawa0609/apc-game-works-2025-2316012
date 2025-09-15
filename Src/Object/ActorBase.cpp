#include "ActorBase.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"

ActorBase::ActorBase():
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
	transform_ = {};
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
	UpdateDebug();
#endif
}

void ActorBase::Draw()
{
	DrawMain();

	DrawTranslucent();

	DrawUI();

#ifdef _DEBUG
	DrawDebug();
#endif 
}

void ActorBase::UpdateMain()
{
}

void ActorBase::UpdateApply()
{
}

void ActorBase::UpdateDebug()
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

void ActorBase::DrawDebug()
{
}

void ActorBase::OnCollision()
{
}
