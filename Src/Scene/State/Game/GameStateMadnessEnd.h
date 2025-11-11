#pragma once
#include "GameStateBase.h"

class GameStateMadnessEnd : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStateMadnessEnd();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStateMadnessEnd() override;

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

};
