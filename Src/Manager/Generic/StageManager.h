#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "../../Template/Singleton.h"

class StageObjectBase;
class MainStage;

class StageManager : public Singleton<StageManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<StageManager>;

public:

	const std::vector<std::string> KEYS =
	{
		"mainStage",
	};

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

private:

	//ステージオブジェクトの管理マップ
	std::unordered_map<std::wstring, std::vector<std::unique_ptr<StageObjectBase>>> stageObjectsMap_;

	//test用ステージオブジェクト
	std::unique_ptr<MainStage> testStage_;

	//コンストラクタ
	StageManager();

	//デストラクタ
	~StageManager() = default;

};
