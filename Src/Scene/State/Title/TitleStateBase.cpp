#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/InputManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/FontManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../SceneTitle.h"
#include "TitleStateBase.h"

TitleStateBase::TitleStateBase(SceneTitle& parent) :
	parent_(parent),
	scnMng_(SceneManager::GetInstance()),
	inputMng_(InputManager::GetInstance()),
	sndMng_(SoundManager::GetInstance()),
	fontMng_(FontManager::GetInstance()),
	resMng_(ResourceManager::GetInstance())
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