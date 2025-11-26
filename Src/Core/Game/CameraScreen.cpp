#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Game/CharacterManager.h"
#include "../../Manager/Game/GameEffectManager.h"
#include "../../Object/Actor/Character/Ghost.h"
#include "../../Utility/UtilityCommon.h"
#include "../PostEffect/PostEffectGameNormal.h"
#include "../PostEffect/PostEffectNightView.h"
#include "CameraScreen.h"

using G_TYPE = GameEffectManager::TYPE;

CameraScreen::CameraScreen() :
	input_(InputManager::GetInstance()),
	charaMng_(CharacterManager::GetInstance()),
	effectMng_(GameEffectManager::GetInstance())
{
	mode_ = SCREEN_MODE::NORMAL;
}

CameraScreen::~CameraScreen()
{
}

void CameraScreen::Load()
{
	// カメラフレームのリソース取得
	cameraFrame_.handleId = resMng_.GetHandle("cameraFrame");

	// アイコンのリソース取得
	switchIcon_.handleIds = resMng_.GetHandles("cameraSwitchIcons");
}

void CameraScreen::Init()
{
	// カメラフレームの初期化
	cameraFrame_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	cameraFrame_.size = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	// スイッチアイコンの初期化
	switchIcon_.pos = { Application::SCREEN_SIZE_X - 80, Application::SCREEN_SIZE_Y - 64 };
	switchIcon_.size = { 128, 128 };
	switchIcon_.scale = 0.7f;

	// モード初期化
	ChangeScreenMode(SCREEN_MODE::NORMAL);
}

void CameraScreen::Update()
{
	// 入力処理
	if (input_.IsTrgDown(InputManager::TYPE::CAMERA_MODE_CHANGE))
	{
		// 状態を整数に変換して値の更新
		int mode = static_cast<int>(mode_) + 1;

		// 最大値を取得
		int max = static_cast<int>(SCREEN_MODE::NIGHT_VIEW);

		// 最大値を超える場合
		if (mode > max)
		{
			// 初期化
			mode = 0;
		}

		// スクリーン状態の切り替え
		ChangeScreenMode(static_cast<SCREEN_MODE>(mode));

		// サウンド再生
		sndMng_.PlaySe(SoundType::SE::CAMERA_MODE_CAHNGE);
	}
}

void CameraScreen::Draw()
{
	// カメラフレームの描画
	cameraFrame_.DrawRota();

	// アイコンの描画
	switchIcon_.DrawRota();
}

void CameraScreen::ChangeScreenMode(const SCREEN_MODE mode)
{
	// モードの切り替え
	mode_ = mode;

	// アイコンの番号を割り当て
	switchIcon_.index = static_cast<int>(mode_);

	// 役者の活動状態の切り替え
	SetActiveActors();

	// エフェクトの切り替え
	G_TYPE type = mode_ == SCREEN_MODE::NORMAL ? G_TYPE::NORMAL : G_TYPE::NIGHT_VIEW;
	effectMng_.ChangeEffect(type);
}

void CameraScreen::SetActiveActors()
{
	// 状態別に活動状態を決定
	bool isActive = mode_ == SCREEN_MODE::NIGHT_VIEW;

	// ゴースト配列の取得
	auto& ghosts = charaMng_.GetCharacters(CharacterManager::TYPE::GHOST);

	// 中身が空の場合終了
	if (ghosts.empty()) { return; }

	// 活動状態の反映
	for (auto& ghost : ghosts)
	{
		ghost->SetIsActive(isActive);
	}
}
