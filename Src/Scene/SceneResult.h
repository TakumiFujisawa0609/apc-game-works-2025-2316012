#pragma once
#include "SceneBase.h"
#include "../Common/CharacterString.h"
#include "../Common/Sprite.h"
#include "../Common/Image.h"

class Timer;
class ControllerTextAnimation;

class SceneResult : public SceneBase
{
public:

	enum class STATE
	{
		START,
		DRUM_ROLL,
		END,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneResult();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneResult();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// ランク別スコア
	static constexpr int RANK_S = 15000;
	static constexpr int RANK_A = 13000;
	static constexpr int RANK_B = 12000;
	static constexpr int RANK_C = 10000;

	// スコア
	int score_;

	// BGM再生判定
	bool isBgm_;

	// 状態
	STATE state_;

	// 更新処理
	std::function<void()> update_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// テキストアニメーション
	std::unique_ptr<ControllerTextAnimation> textAnimation_;

	// テキスト
	CharacterString titleText_;

	// スコアテキスト
	CharacterString scoreText_;

	// コメントテキスト
	CharacterString commentText_;

	// ランク
	Sprite rank_;

	// ゴースト
	Image ghost_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	// 状態別更新処理
	void UpdateStart();
	void UpdateDrumRoll();
	void UpdateEnd();
};