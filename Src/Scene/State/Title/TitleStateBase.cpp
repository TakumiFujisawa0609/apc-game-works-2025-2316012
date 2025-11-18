#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Resource/SoundManager.h"
#include "../../SceneTitle.h/"
#include "TitleStateBase.h"

TitleStateBase::TitleStateBase(SceneTitle& parent) :
	parent_(parent),
	scnMng_(SceneManager::GetInstance()),
	inputMng_(InputManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
}

TitleStateBase::~TitleStateBase()
{
}

void TitleStateBase::Init()
{
}

void TitleStateBase::Update()
{
}

void TitleStateBase::Draw()
{
}