#include <DxLib.h>
#include <time.h>
#include <EffekseerForDXLib.h>
#include "Manager/Generic/SceneManager.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Resource/ResourceManager.h"
#include "Manager/Resource/FontManager.h"
#include "FpsControl/FpsControl.h"
#include "Application.h"

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_FONT = "Data/Font/";
const std::string Application::PATH_TEXT = "Data/Text/";
const std::string Application::PATH_JSON = "Data/JSON/";
const std::string Application::PATH_CSV = "Data/CSV/";
const std::string Application::PATH_SHADER = "Data/Shader/";

bool Application::Init()
{

	// アプリケーションの初期設定
	SetWindowText(L"BaseProject.ver1.2");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return false;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();			//生成
	InputManager::GetInstance().Init();		//初期化

	// リソース管理初期化
	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();	

	// シーン管理初期化
	SceneManager::CreateInstance();		
	SceneManager::GetInstance().Init();	

	// FPS初期化
	fps_ = std::make_unique<FpsControl>();
	fps_->Init();

	// ランダム生成数字の初期化
	SRand((unsigned int)time(NULL));

	return true;
}

void Application::Run()
{
	LONGLONG time = GetNowHiPerformanceCount();

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//フレームレートを更新
		if (!fps_->UpdateFrameRate()) continue;

		//更新処理
		inputManager.Update();
		sceneManager.Update();

		//描画処理
		sceneManager.Draw();

		//フレームレート計算
		fps_->CalcFrameRate();

		ScreenFlip();
	}
}

bool Application::Release()
{
	//各クラスのリソースの破棄	
	SceneManager::GetInstance().Release();	
	ResourceManager::GetInstance().Release();
	InputManager::GetInstance().Release();

	//インスタンスの破棄
	SceneManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();
	
	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		return false;
	}

	return true;
}

Application::Application()
{
	fps_ = nullptr;
}

void Application::InitEffekseer()
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
