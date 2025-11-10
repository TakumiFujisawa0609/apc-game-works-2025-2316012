#include <DxLib.h>
#include <cassert>
#include "../../Resource/ResourceSound.h"
#include "ResourceManager.h"
#include "SoundManager.h"

// 音源種類の省略形
using TYPE = SoundType::TYPE;
using BGM = SoundType::BGM;
using SE = SoundType::SE;

void SoundManager::SceneChangeResources()
{
	// 一時マップ
	std::unordered_map<BGM, SoundData> tempBgmMap;
	std::unordered_map<SE, SoundData> tempSeMap;

	// シーン用リソースを取得
	std::unordered_map<std::string, ResourceSound*> resourceMap = ResourceManager::GetInstance().GetSceneSounds();

	// リソースが空の場合は終了
	if (resourceMap.empty())
	{
		return;
	}

	// 新規リソースを追加
	for (auto it = resourceMap.begin(); it != resourceMap.end(); )
	{
		// 音源の種類を取得
		TYPE type = it->second->GetSoundType();

		// サウンドデータの格納
		SoundData soundData = {};
		soundData.playType = GetPlayType(type);		// 再生方法
		soundData.handle = it->second->GetHandle();	// ハンドル

		// 音源の種類別に格納
		if (type == TYPE::BGM)
		{
			// キーから列挙型値に変換
			const BGM bgmKey = SoundType::KEY_BGM_MAP.at(it->first);

			// 格納
			tempBgmMap.emplace(bgmKey, soundData);
		}
		else if (type == TYPE::SE)
		{
			// キーから列挙型値に変換
			const SE seKey = SoundType::KEY_SE_MAP.at(it->first);

			// 格納
			tempSeMap.emplace(seKey, soundData);
		}

		// 次へ
		++it;
	}

	// 読み込み済みマップを更新
	loadedBgmMap_ = tempBgmMap;
	loadedSeMap_ = tempSeMap;
}

void SoundManager::PlayBgm(const SoundType::BGM key, const bool topPos)
{
	//音源があるか確認
	auto it = loadedBgmMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedBgmMap_.end()&& "追加していない音源を再生しようとしています");

	//再生
	PlaySoundMem(loadedBgmMap_[key].handle, loadedBgmMap_[key].playType, topPos);
}

void SoundManager::PlaySe(const SoundType::SE key, const bool topPos)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end()&& "追加していない音源を再生しようとしています");

	//再生
	PlaySoundMem(loadedSeMap_[key].handle, loadedSeMap_[key].playType, topPos);
}

void SoundManager::StopBgm(const SoundType::BGM key)
{
	//音源があるか確認
	auto it = loadedBgmMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedBgmMap_.end()&& "追加していない音源を停止しようとしています");

	//停止
	StopSoundMem(loadedBgmMap_[key].handle);
}

void SoundManager::StopSe(const SoundType::SE key)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end()&& "追加していない音源を停止しようとしています");

	//停止
	StopSoundMem(loadedSeMap_[key].handle);
}

void SoundManager::StopAllSe()
{
	for (auto& se : loadedSeMap_)
	{
		if (CheckSoundMem(se.second.handle))
		{
			StopSoundMem(se.second.handle);
		}
	}
}

void SoundManager::ChangeVolumeByType(const int volume, const TYPE type)
{	
	constexpr int MAX = 255;	//最大値
	constexpr int DIV = 100;	//分割数

	//指定した種類がBGMの場合
	if (TYPE::BGM == type)
	{
		for (const auto& [id, sound] : loadedBgmMap_)
		{
			//音源の音量変更
			ChangeVolumeSoundMem(MAX * volume / DIV, sound.handle);
		}
	}
	// SEの場合
	else if (TYPE::SE == type)
	{
		for (const auto& [id, sound] : loadedSeMap_)
		{
			//音源の音量変更
			ChangeVolumeSoundMem(MAX * volume / DIV, sound.handle);
		}
	}
}

void SoundManager::Release()
{
	loadedBgmMap_.clear();
	loadedSeMap_.clear();
}

int SoundManager::GetPlayType(const TYPE soundType)
{
	switch (soundType)
	{
	case TYPE::SE:
		return DX_PLAYTYPE_BACK;
		break;

	case TYPE::BGM:
		return DX_PLAYTYPE_LOOP;
		break;

	default:
		break;
	}
	return -1;
}

SoundManager::SoundManager()
{
}
