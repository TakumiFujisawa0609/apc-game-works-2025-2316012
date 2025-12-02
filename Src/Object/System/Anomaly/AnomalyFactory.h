#pragma once
#include <memory>
#include "../../../Manager/Game/AnomalyManager.h"

class AnomalyBase;
class AnomalyAppearance;
class AnomalyBloodyRoom;
class AnomalyChairMountain;
class AnomalyGhost;
class AnomalyGrassRoom;
class AnomalyPainting;

class AnomalyFactory
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyFactory();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyFactory();

	/// <summary>
	/// 異変の生成
	/// </summary>
	/// <returns>異変</returns>
	std::unique_ptr<AnomalyBase> Create(const AnomalyManager::TYPE type);

private:

	// 生成処理の管理マップ
	std::map<AnomalyManager::TYPE, std::function<std::unique_ptr<AnomalyBase>()>> createAnomalyMap_;

	// コライダー生成処理の登録処理
	void RegisterCreate(const AnomalyManager::TYPE type, std::function<std::unique_ptr<AnomalyBase>()> func);

	// 各種異変ごとの生成処理
	std::unique_ptr<AnomalyAppearance> CreateAppearance();
	std::unique_ptr<AnomalyBloodyRoom> CreateBloodyRoom();
	std::unique_ptr<AnomalyChairMountain> CreateChairMountain();
	std::unique_ptr<AnomalyGhost> CreateGhost();
	std::unique_ptr<AnomalyGrassRoom> CreateGrassRoom();
	std::unique_ptr<AnomalyPainting> CreatePainting();

};

