#pragma once
#include <unordered_map>
#include <string>
#include "../../Template/Singleton.h"
#include "../../Resource/ResourceBase.h"

class ResourceManager : public Singleton<ResourceManager>
{

	friend class Singleton<ResourceManager>;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// リソースの完全解放
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// シーン変更の処理
	/// </summary>
	/// <param name="nextSceneId">次シーンのID</param>
	void SceneChangeResource(const int nextSceneId);

	/// <summary>
	/// 指定したキーのリソースを返す
	/// </summary>
	const ResourceBase& GetResource(const std::string& key) const;

private

	// リソース管理の対象
	std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resourcesMap_;

	// 読み込み済みリソース
	std::unordered_map<std::string, ResourceBase&> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void) = default;
};