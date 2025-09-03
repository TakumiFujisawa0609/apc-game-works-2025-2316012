#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>

void SoundManager::Add(const int sound, const SOUNDTYPE type)
{
	//’Ç‰ÁÏ‚İ‚©Šm”F
	auto it = loadedMap_.find(sound);

	//’Ç‰Á‚µ‚Ä‚È‚¢ê‡
	if (it == loadedMap_.end())
	{
		//’l‚ÌŠi”[
		SoundData ret = {};
		ret.handle = sound;
		ret.soundType = type;
		ret.playType = GetSoundType(ret.soundType);
		loadedMap_.emplace(sound, ret);
	}
}

void SoundManager::Play(const int sound, const bool topPos)
{
	//‰¹Œ¹‚ª‚ ‚é‚©Šm”F
	auto it = loadedMap_.find(sound);

	//‰¹Œ¹‚ª‚È‚¢ê‡‹­§’â~
	assert(it != loadedMap_.end(), "’Ç‰Á‚µ‚Ä‚¢‚È‚¢‰¹Œ¹‚ğÄ¶‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
	
	//Ä¶
	PlaySoundMem(loadedMap_[sound].handle, loadedMap_[sound].playType, topPos);	
}

void SoundManager::Stop(const int sound)
{
	//‰¹Œ¹‚ª‚ ‚é‚©Šm”F
	auto it = loadedMap_.find(sound);

	//‰¹Œ¹‚ª‚È‚¢ê‡‹­§’â~
	assert(it != loadedMap_.end(), "’Ç‰Á‚µ‚Ä‚¢‚È‚¢‰¹Œ¹‚ğ’â~‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");

	//’â~
	StopSoundMem(loadedMap_[sound].handle);
}

void SoundManager::ChangeVolumeByType(const int volume, const SOUNDTYPE type)
{	
	constexpr int MAX = 255;	//Å‘å’l
	constexpr int DIV = 100;	//•ªŠ„”

	for (const auto& [id, sound] : loadedMap_)
	{
		//‰¹Œ¹‚Ìí—Ş‚ªˆê’v‚·‚é‚Æ‚«
		if (sound.soundType == type)
		{
			//‰¹Œ¹‚Ì‰¹—Ê•ÏX
			ChangeVolumeSoundMem(MAX * volume / DIV, sound.handle);
		}
	}
}

void SoundManager::Release()
{
	loadedMap_.clear();
}

int SoundManager::GetSoundType(const SOUNDTYPE soundType)
{
	switch (soundType)
	{
	case SOUNDTYPE::SE:
		return DX_PLAYTYPE_BACK;
		break;

	case SOUNDTYPE::BGM:
		return DX_PLAYTYPE_LOOP;
		break;

	default:
		break;
	}
}

SoundManager::SoundManager()
{
}
