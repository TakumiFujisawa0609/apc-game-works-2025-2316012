#pragma once
#include <vector>
#include "ResourceBase.h"

class ResourceSound : public ResourceBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="path">パス名</param>
	/// <param name="sceneId">シーンID</param>
	ResourceSound(const RESOURCE_TYPE type, const std::wstring path, const int sceneId);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceSound()override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release()override;

	/// <summary>
	/// ハンドルを返す
	/// </summary>
	/// <returns>ハンドルを返す</returns>
	const int GetHandle() override;

private:

	//複製した音声を管理
	std::vector<int> duplicateSounds_;
};

