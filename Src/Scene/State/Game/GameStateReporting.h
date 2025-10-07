#pragma once
#include "GameStateBase.h"

class GameStateReporting : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStateReporting();

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


};

