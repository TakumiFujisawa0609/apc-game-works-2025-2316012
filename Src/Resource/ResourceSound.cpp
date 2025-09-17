#include <DxLib.h>
#include "ResourceSound.h"

ResourceSound::ResourceSound(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId):
	ResourceBase(type,path,sceneId)
{
}

ResourceSound::~ResourceSound()
{
}

void ResourceSound::Load()
{
	handleId_ = LoadSoundMem(path_.c_str());
}

void ResourceSound::Release()
{
	if (!duplicateSounds_.empty())
	{
		for (auto snd : duplicateSounds_)
		{
			DeleteSoundMem(snd);
		}
	}

	DeleteSoundMem(handleId_);
}

const int ResourceSound::GetHandle()
{
	int sound = DuplicateSoundMem(handleId_);
	duplicateSounds_.push_back(sound);
	return sound;
}
