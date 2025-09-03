#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

/// <summary>
/// 再生種類
/// </summary>
enum class SOUNDTYPE
{
	BGM,
	SE,
	MAX
};

class SoundManager : public Singleton<SoundManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<SoundManager>;

public:

	//デフォルトの音量(100だと少し大きいため)
	static constexpr int DEFAULT_VOLUME = 70;

	/// <summary>
	/// 音源の追加
	/// </summary>
	/// <param name="sound">音源</param>
	/// <param name="type">音源の種類</param>
	void Add(const int sound, const SOUNDTYPE type);

	/// <summary>
	/// 音源の再生
	/// </summary>
	/// <param name="sound">音源</param>
	/// <param name="topPos">再生位置を戻すか</param>
	void Play(const int sound, const bool topPos = true);

	/// <summary>
	/// 音源の停止
	/// </summary>
	/// <param name="sound">音源</param>
	void Stop(const int sound);

	/// <summary>
	/// サウンドの音量調整
	/// </summary>	
	/// <param name="volume">音量(0～100まで)</param>
	/// <param name="type">音源の種類</param>
	void ChangeVolumeByType(const int volume, const SOUNDTYPE type);

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
		int handle;			//ハンドル
		int playType;		//再生方法
		SOUNDTYPE soundType;//音源種類
	};

	//読み込みリソース管理
	std::unordered_map<int, SoundData> loadedMap_;

	//再生種類を返す
	int GetSoundType(const SOUNDTYPE soundType);

	SoundManager();
	~SoundManager() = default;
};

