#pragma once
#include "GameStateBase.h"

class ReportSystem;

class GameStateReporting : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親のインスタンス</param>
	GameStateReporting(SceneGame& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStateReporting() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// レポート
	ReportSystem* report_;

	// 状態遷移確認
	void CheckChangeState() override;
};

