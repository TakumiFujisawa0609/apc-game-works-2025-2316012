#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../../Template/Singleton.h"
#include "../../Object/Actor/Stage/StageObjectBase.h"

class GrassRoom;
class StageObjectBase;
class StageMain;
class Grass;

// JSON名前空間
using Json = nlohmann::json;

class StageManager : public Singleton<StageManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<StageManager>;

public:

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 不要のオブジェクトの削除
	/// </summary>
	void Sweep();

	/// <summary>
	/// 草の削除
	/// </summary>
	void DeleteGrass();

	/// <summary>
	/// 全てのオブジェクトの活動状態を設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsActiveByAllObjects(const bool isActive);

	/// <summary>
	/// ステージオブジェクトの追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="stageObject">ステージオブジェクト/param>
	void Add(const std::string& type, std::unique_ptr<StageObjectBase> stageObject);

	/// <summary>
	/// 草の追加
	/// </summary>
	/// <param name="grass"></param>
	void AddGrass(std::unique_ptr<Grass> grass);

	/// <summary>
	/// 指定したステージオブジェクトの配列を返す
	/// </summary>
	/// <param name="key">ステージの種類</param>
	/// <returns>ステージオブジェクトの配列</returns>
	std::vector<std::unique_ptr<StageObjectBase>>& GetStageObjects(const std::string& key) { return stageObjectsMap_[key]; }

	/// <summary>
	/// 指定したステージオブジェクトのコライダー情報を返す
	/// </summary>
	/// <param name="key">ステージの種類</param>
	/// <returns>コライダーの情報</returns>
	const Json& GetStageObjectColliderParam(const std::string& key) const;

private:

	// 配置用ファイル名
	const std::string STAGE_FILE_NAME = "Stage";

	// コライダー用ファイル名
	const std::string COLLIDER_FILE_NAME = "StageObjectsCollider";	
	
	// 部屋の名前配列
	const std::vector<std::string> MAIN_STAGES =
	{
		"RoomA",
		"RoomB",
		"RoomC",
		"RoomD",
		"RoomE",
		"Roof",
		"CorriderA",
		"CorriderB",
		"CorriderC",
	};

	// 必ず描画を行うタグ
	const std::string TAG_ABSOLUTE = "absolute";

	std::unordered_map<std::string, std::vector<Json>> paramStageMap_;

	// 描画するタグリスト
	std::vector<std::string> drawTagList_;

	// 部屋配列
	std::vector<StageMain*> mainStages_;

	// 通常描画リスト
	std::vector<StageObjectBase*> opaqueList_;

	// 透過描画リスト
	std::vector<StageObjectBase*> translucentList_;

	// 草の部屋
	std::vector<std::unique_ptr<Grass>> grassList_;

	// ステージオブジェクトの管理マップ
	std::unordered_map<std::string, std::vector<std::unique_ptr<StageObjectBase>>> stageObjectsMap_;

	// コライダー情報の管理マップ
	std::unordered_map<std::string, std::vector<Json>> stageObjectColliserInfoMap_;

	// メインルームがカメラ範囲内か調べる
	void CheckMainRoomInClipCameraView();

	//コンストラクタ
	StageManager();

	//デストラクタ
	~StageManager();

};