#include "../../Application.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/CharacterManager.h"
#include "../../Manager/Resource/ResourceManager.h"
#include "../../Object/Actor/Character/Ghost.h"
#include "../PostEffect//PostEffectGameNormal.h"
#include "../PostEffect//PostEffectNightView.h"
#include "CameraScreen.h"

CameraScreen::CameraScreen() :
	input_(InputManager::GetInstance()),
	charaMng_(CharacterManager::GetInstance())
{
	postEffectScreen_ = -1;
	mode_ = SCREEN_MODE::NORMAL;
}

CameraScreen::~CameraScreen()
{
	// エフェクトスクリーンを削除
	DeleteGraph(postEffectScreen_);
}

void CameraScreen::Load()
{
	// カメラフレームのリソース取得
	cameraFrame_.handleId = resMng_.GetHandle("cameraFrame");

	// 生成
	auto normalEffect = std::make_unique<PostEffectGameNormal>();
	effectMap_.emplace(SCREEN_MODE::NORMAL, std::move(normalEffect));

	auto nightViewEffect = std::make_unique<PostEffectNightView>();
	effectMap_.emplace(SCREEN_MODE::NIGHT_VIEW, std::move(nightViewEffect));

	// 読み込み処理
	for (auto& effect : effectMap_)
	{
		effect.second->Load();
	}

	// ポストエフェクトスクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void CameraScreen::Init()
{
	// カメラフレームの初期化
	cameraFrame_.pos = { 0,0, };
	cameraFrame_.size = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	// 初期化処理
	for (auto& effect : effectMap_)
	{
		effect.second->Init();
	}

	// モード初期化
	mode_ = SCREEN_MODE::NORMAL;
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

		// デバッグ時は最大を変更
#ifdef _DEBUG	
		max = static_cast<int>(SCREEN_MODE::DEBUG);
#endif 
		// 最大値を超える場合
		if (mode > max)
		{
			// 初期化
			mode = 0;
		}

		// スクリーン状態の切り替え
		ChangeScreenMode(static_cast<SCREEN_MODE>(mode));
	}
}

void CameraScreen::Draw()
{
	if (mode_ == SCREEN_MODE::DEBUG) { return; }

	//スクリーンの設定
	SetDrawScreen(postEffectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// エフェクトを描画
	effectMap_[mode_]->Draw();

	// メインに戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// 描画
	DrawGraph(0, 0, postEffectScreen_, false);

	// カメラフレームの描画
	cameraFrame_.DrawExtend();
}

void CameraScreen::ChangeScreenMode(const SCREEN_MODE mode)
{
	// モードの切り替え
	mode_ = mode;

	// 役者らの活動状態の切り替え
	SetActiveActors();
}

void CameraScreen::SetActiveActors()
{
	if(mode_ == SCREEN_MODE::DEBUG) { return; }

	// 状態別に活動状態を決定
	bool isActive = mode_ == SCREEN_MODE::NORMAL;

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
