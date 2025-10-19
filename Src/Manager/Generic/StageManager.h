#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../../Template/Singleton.h"

class StageObjectBase;

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
	/// 指定したステージオブジェクトの配列を返す
	/// </summary>
	/// <param name="key">ステージの種類</param>
	/// <returns></returns>
	std::vector<std::unique_ptr<StageObjectBase>>& GetStageObjects(const std::string& key) { return stageObjectsMap_.at(key); }

private:

	// 配置用ファイル名
	const std::string STAGE_FILE_NAME = "Stage";

	// コライダー用ファイル名
	const std::string COLLIDER_FILE_NAME = "StageObjectsCollider";

	//ステージオブジェクトの管理マップ
	std::unordered_map<std::string, std::vector<std::unique_ptr<StageObjectBase>>> stageObjectsMap_;

	//コンストラクタ
	StageManager();

	//デストラクタ
	~StageManager();

};
