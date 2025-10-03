#pragma once
class StageManager : public Singleton<StageManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<StageManager>;

public:

	// ステージオブジェクト
	const std::vector<std::string> NAME_LIST =
	{
		"mainStage",
		"Alembic02",
		"Board"
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

	const std::string FILEN_NAME = "Stage";

	// パラメータ読み込み
	std::unique_ptr<ParameterLoad> paramLoad_;

	//ステージオブジェクトの管理マップ
	std::unordered_map<std::string, std::vector<std::unique_ptr<StageObjectBase>>> stageObjectsMap_;

	//コンストラクタ
	StageManager();

	//デストラクタ
	~StageManager() = default;

};

