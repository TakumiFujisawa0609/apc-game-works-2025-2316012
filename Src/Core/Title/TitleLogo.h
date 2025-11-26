#pragma once
#include "CoreTitleBase.h"

class Timer;

class TitleLogo : public CoreTitleBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleLogo();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleLogo();

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

	// 分割数
	static constexpr float DIV_NUM = 5.0f;

	// グリッチ速度
	const float GLITCH_SPEED = 40.0f;

	// sin関数の周期を制御する周波数
	const float GLITCH_CYCLE_RATE = 5.0f;

	// 振幅のスケール
	const float GLITCH_AMPLITUDE_SCALE = 5.0f;

	// 範囲調整
	const float GLITCH_OFFSET = 0.5f;

	// 通常描画時間
	const float NORMAL_DRAW_TIME = 3.0f;

	// グリッチ中の描画
	const float GLITCH_DRAW_TIME = 1.0f;

	// ロゴハンドル
	int logo_;

	// 描画位置
	Vector2 pos_;

	// ステップ
	float step_;

	// タイマー
	std::unique_ptr<Timer> timer_;
};