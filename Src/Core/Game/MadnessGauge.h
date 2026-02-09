#pragma once
#include "CoreGameBase.h"
#include "../Common/Image.h"
#include "../Common/CharacterString.h"

class Player;

class MadnessGauge : public CoreGameBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ</param>
	/// <param name="player">プレイヤー</param>
	MadnessGauge(const Json& param, const Player& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MadnessGauge();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	// ゲージの位置
	const Vector2 GAUGE_POS;

	// ピンの位置
	const int PIN_POS_X;
	const int PIN_POS_Y_MAX;	// ゲージ最大時の位置
	const int PIN_POS_Y_MIN;	// ゲージ最小時の位置

	// テキストの位置
	const Vector2 STR_SUN_POS;
	const Vector2 STR_DIE_POS;

	// テキスト
	const std::wstring STR_SUN;
	const std::wstring STR_DIE;

	// フォント名
	const std::string FONT_NAME;

	// フォントサイズ
	const int FONT_SIZE;

	// フォント太さ
	const int FONT_THICK;

	// プレイヤーの参照
	const Player& player_;

	// ゲージ
	Image imgGauge_;

	// ピン
	Image imgPin_;

	// ゲージ用ストリング
	CharacterString strSun_;

	// ゲージ用ストリング
	CharacterString strDie_;
};