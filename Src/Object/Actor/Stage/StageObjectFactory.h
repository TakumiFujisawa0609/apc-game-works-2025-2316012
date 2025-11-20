#pragma once
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

// JSON名前空間
using Json = nlohmann::json;

class StageObjectBase;
class StageGimmickObjectBase;
class StageMesh;
class StageMain;
class Painting;
class Carpet;
class GrassRoom;

class StageObjectFactory
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageObjectFactory();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageObjectFactory();

	/// <summary>
	/// ステージオブジェクトの生成
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	/// <returns>生成したオブジェクト</returns>
	std::unique_ptr<StageObjectBase> Create(const std::string& key, const Json& mapParam, const Json& colliderParam);

private:

	// 生成処理の管理マップ
	std::map<std::string, std::function<std::unique_ptr<StageObjectBase>(const std::string&, const Json&, const Json&)>> createObjectMap_;

	// コライダー生成処理の登録処理
	void RegisterCreate(const std::string& type, std::function<std::unique_ptr<StageObjectBase>(const std::string&, const Json&, const Json&)> func);

	// 各種オブジェクトごとの生成処理
	std::unique_ptr<StageObjectBase> CreateStageObject(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<StageGimmickObjectBase> CreateStageObjectGimmick(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<Painting> CreatePainting(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<StageMesh> CreateStageMesh(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<StageMain> CreateStageMain(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<Carpet> CreateCarpet(const std::string& key, const Json& mapParam, const Json& colliderParam);
	std::unique_ptr<GrassRoom> CreateGrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam);
};

