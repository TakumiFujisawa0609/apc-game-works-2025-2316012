#pragma once
#include <vector>
#include "CoreTitleBase.h"

class Timer;

class TitleOrb : public CoreTitleBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleOrb();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleOrb() override;

	/// <summary>
	/// 初期化
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

	// ランダム用スピード値
	static constexpr int SPEED_MIN = 1;
	static constexpr int SPEED_MAX = 3;
	
	// ランダム用画像拡大率
	static constexpr int SCALE_MIN = 7;
	static constexpr int SCALE_MAX = 15;

	// ランダム用生成間隔
	static constexpr float CREATE_INTERVAL_MIN = 0.1f;

	// 生成位置Y
	static constexpr int CREATE_POS_Y = 750;

	// 生成間隔
	static constexpr float CREATE_INTERVAL = 1.0f;

	// 削除ライン
	static constexpr int DELETE_POS_X = 1320;
	static constexpr int DELETE_POS_Y = -50;

	// 画像ハンドル
	int imgOrb_;

	// オーブ情報
	struct Orb
	{
		float speedX = 0.0f;	// 速度
		float speedY = 0.0f;	// 速度
		Image img = {};			// 画像
	};

	// タイマー
	std::unique_ptr<Timer> timer_;

	// オーブ
	std::vector<Orb> orbs_;

	// 生成
	void CreateOrb();

};

