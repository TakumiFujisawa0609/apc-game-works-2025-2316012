#include "../../Application.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Game/GameStateManager.h"
#include "../../Manager/Game/CharacterManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Object/Actor/Character/Player.h"
#include "MadnessGauge.h"

MadnessGauge::MadnessGauge(const Player& player) :
	player_(player)
{
}

MadnessGauge::~MadnessGauge()
{
}

void MadnessGauge::Load()
{
	// 画像ハンドルの取得
	gauge_.handleId = resMng_.GetHandle("madnessGauge");
	pin_.handleId = resMng_.GetHandle("madnessGaugePin");

	// ゲージの取得
	gaugeSize_ = { 90, 384 };
}

void MadnessGauge::Init()
{
	// 位置の設定
	gauge_.pos = { Application::SCREEN_SIZE_X - 75 , Application::SCREEN_HALF_Y };
	pin_.pos = { gauge_.pos.x, gauge_.pos.y + gaugeSize_.y / 2 };
	pinMinPos_ = pin_.pos;
}

void MadnessGauge::Update()
{
	// 現在の狂気値を取得
	int value = player_.GetMadnessValue();

	// 1未満の場合
	if (value < 1)
	{
		// 最小位置に設定
		pin_.pos = pinMinPos_;
	}
	else
	{
		// 変化量の計算
		float step = (float)gaugeSize_.y / (float)Player::MADNESS_MAX * value;

		// 座標の更新
		pin_.pos.y = pinMinPos_.y - static_cast<int>(step);
	}
}

void MadnessGauge::Draw()
{
	// ゲージの描画
	gauge_.DrawRota();
	
	// ピンの描画
	pin_.DrawRota();
}
