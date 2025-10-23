#include "../../Application.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../PostEffect//PostEffectGameNormal.h"
#include "CameraScreen.h"

CameraScreen::CameraScreen() :
	input_(InputManager::GetInstance())
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
	// 生成
	auto normalEffect = std::make_unique<PostEffectGameNormal>();
	effectMap_.emplace(SCREEN_MODE::NORMAL, std::move(normalEffect));

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
		int mode = static_cast<int>(mode_);
		int max = static_cast<int>(SCREEN_MODE::NIGHT_VIEW);
#ifdef _DEBUG	
		max = static_cast<int>(SCREEN_MODE::DEBUG);
#endif 
		if (mode > max)
		{
			mode = 0;
		}

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

	//描画
	DrawGraph(0, 0, postEffectScreen_, false);
}

void CameraScreen::ChangeScreenMode(const SCREEN_MODE mode)
{
	mode_ = mode;

	effectMap_[mode_]->Init();
}
