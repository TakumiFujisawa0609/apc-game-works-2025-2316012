#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"
#include "../../Scene/SceneGame.h"
#include "../../Core/Game/Message.h"

class CoreGameBase;

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
		MESSAGE,		// メッセージ
		GAME_TIMER,		// タイマー
		MANUAL,			// 操作説明
		SCREEN,			// スクリーン
		MAX,
	};

	// システムの名前リスト
	const std::vector<std::string> NAME_LIST =
	{
		"report",
		"message",
		"gameTimer",
		"manual",
		"screen",
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
	/// メッセージの変更
	/// </summary>
	/// <param name="type">メッセージの変更種類</param>
	void ChangeMessage(const Message::TYPE type);

	/// <summary>
	/// 各システムの活動状態の変更
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="isActive">活動判定</param>
	void SetIsActiveSystem(const TYPE type, const bool isActive);

	/// <summary>
	/// 指定した参照インスタンスを返す
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	CoreGameBase& GetGamsSystem(const TYPE type) { return *systemsMap_[type]; }

private:

	// ファイル名
	const std::string FILE_NAME = "ParameterSystems";

	// システムごとにインスタンスを管理するマップ
	std::unordered_map<TYPE, std::unique_ptr<CoreGameBase>> systemsMap_;

	// コンストラクタ
	GameSystemManager();

	// デストラクタ
	~GameSystemManager();
};