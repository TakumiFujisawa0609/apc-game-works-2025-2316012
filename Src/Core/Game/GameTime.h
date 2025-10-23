#pragma once
#include <memory>
#include "CoreGameBase.h"

class Timer;
class GameStateManager;

class GameTime : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameTime();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameTime() override;

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
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// 状態管理クラスの参照
	GameStateManager& stateMng_;

	// 日付情報
	CharacterString todayText_;

	// 時間情報
	CharacterString timeText_;

	// 時間
	std::unique_ptr<Timer> timer_;

	// 年月日の文字列を返す
	std::wstring GetYmdWstring();
};