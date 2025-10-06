#pragma once
#include <functional>
#include <unordered_map>
#include "../../CoreBase.h"

class ReportSystem : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ReportSystem();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReportSystem() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		WAIT,		// 待機
		REPORT,		// 報告
		REPORTING,	// 報告中
		MISS,		// 失敗
		COMPLETE,	// 完了
	};

	// 状態
	STATE state_;

	// 状態別更新処理管理マップ
	std::unordered_map<STATE, std::function<void()>> updateMap_;	

	// 状態別描画処理管理マップ
	std::unordered_map<STATE, std::function<void()>> drawMap_;	

	// 処理の登録
	void RegisterStateFunc(const STATE state, std::function<void()> update, std::function<void()> draw);

	// 状態別更新処理
	void UpdateWait();
	void UpdateReport();
	void UpdateReporting();
	void UpdateMiss();
	void UpdtateComplete();

	// 状態別描画処理
	void DrawWait();
	void DrawReport();
	void DrawReporting();
	void DrawMiss();
	void DrawComplite();

};

