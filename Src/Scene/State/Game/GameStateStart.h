#pragma once
#include <memory>
#include <functional>
#include "GameStateBase.h"

class PixelMaterial;
class PixelRenderer;
class Timer;

class GameStateStart : public GameStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStateStart();

	/// <summary>
	/// デストラクタ　
	/// </summary>
	~GameStateStart() override;

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

	// 待機時間
	static constexpr float WAIT_TIME = 1.0f;

	// ゲーム開始時間
	static constexpr float GAME_START_TIME = 5.0f;

	// フェード用係数
	static constexpr float FADE_COEFFICIENT = 1.9f;

	// テクスチャサイズ
	static constexpr int TEXTURE_SIZE_X = 960;
	static constexpr int TEXTURE_SIZE_Y = 130;

	// フェード用ステップ
	float fadeStep_;

	// マテリアル
	std::unique_ptr<PixelMaterial> material_;

	// レンダラー
	std::unique_ptr<PixelRenderer> renderer_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// 更新処理
	std::function<void()> update_;

	// 状態別更新処理
	void UpdateWait();
	void UpdateEffect();
};