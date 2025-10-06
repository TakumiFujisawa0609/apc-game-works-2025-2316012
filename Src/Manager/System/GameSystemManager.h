#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

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

private:

	std::unordered_map<TYPE, std::unique_ptr<CoreBase>> systemsMap_;

	// コンストラクタ
	GameSystemManager();

	// デストラクタ
	~GameSystemManager();
};