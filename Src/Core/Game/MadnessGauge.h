#pragma once
#include "CoreGameBase.h"
#include "../../Common/Sprite.h"

class Player;

class MadnessGauge : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MadnessGauge(const Player& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MadnessGauge() override;

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

	// プレイヤー
	const Player& player_;

	// ゲージ
	Sprite gauge_;

	// ピン
	Sprite pin_;

	// ゲージサイズ
	Vector2 gaugeSize_;

	// 狂気値最小の時の位置
	Vector2 pinMinPos_;
};