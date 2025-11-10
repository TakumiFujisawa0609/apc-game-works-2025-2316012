#pragma once
#include <string>
#include <unordered_map>
#include "../../Template/Singleton.h"
#include "SoundType.h"

class SoundManager : public Singleton<SoundManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<SoundManager>;

public:

	//デフォルトの音量(100だと少し大きいため)
	static constexpr int DEFAULT_VOLUME = 70;
	
	/// <summary>
	/// シーン遷移する際のリソースの変更
	/// </summary>
	void SceneChangeResources();

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="key">BGMのキー</param>
	/// <param name="topPos">再生位置を戻すか</param>
	void PlayBgm(const SoundType::BGM key, const bool topPos = true);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="key">BGMのキー</param>
	/// <param name="topPos">再生位置を戻すか</param>
	void PlaySe(const SoundType::SE key, const bool topPos = true);

	/// <summary>
	/// BGMの停止
	/// </summary>
	/// <param name="sound">BGMのキー</param>
	void StopBgm(const SoundType::BGM key);

	/// <summary>
	/// SEの停止
	/// </summary>
	/// <param name="key">SEのキー</param>
	void StopSe(const SoundType::SE key);

	/// <summary>
	/// 全ての再生中のSEの停止
	/// </summary>
	void StopAllSe();

	/// <summary>
	/// サウンドの音量調整
	/// </summary>	
	/// <param name="volume">音量(0～100まで)</param>
	/// <param name="type">音源の種類</param>
	void ChangeVolumeByType(const int volume, const SoundType::TYPE type);

	/// <summary>
	/// 読み込んだ音源の解放
	/// </summary>
	void Release();

private:

	/// <summary>
	/// サウンド情報管理
	/// </summary>
	struct SoundData
	{
		int handle;					// 音源
		int playType;				// 再生方法
	};

	//読み込みリソース管理
	std::unordered_map<SoundType::BGM, SoundData> loadedBgmMap_;	// BGM
	std::unordered_map<SoundType::SE, SoundData> loadedSeMap_;	// BGM

	//再生種類を返す
	int GetPlayType(const SoundType::TYPE soundType);

	// コンストラクタ
	SoundManager();

	// デストラクタ
	~SoundManager() = default;
};
