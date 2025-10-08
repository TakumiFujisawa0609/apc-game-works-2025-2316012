#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"
#include "../../Scene/SceneGame.h"

class CoreBase;

class GameSystemManager : public Singleton<GameSystemManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<GameSystemManager>;

public:

	/// <summary>
	/// 種類
	/// </summary>
	enum class TYPE
	{
		REPORT,			// 報告処理
		REPORTING,		// 報告中の処理
		TIMER,			// タイマー
		KEY_MANUAL,		// 操作説明
		CAMERA_CHANGE,	// カメラ切り替え
		MAX,
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

	/// <summary>
	/// 指定した参照インスタンスを返す
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	CoreBase& GetGamsSystem(const TYPE type) { return *systemsMap_[type]; }

private:

	// システムごとにインスタンスを管理するマップ
	std::unordered_map<TYPE, std::unique_ptr<CoreBase>> systemsMap_;

	// コンストラクタ
	GameSystemManager();

	// デストラクタ
	~GameSystemManager();
};