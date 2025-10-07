#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/StageManager.h"
#include "../Manager/Generic/CharacterManager.h"
#include "../Manager/Generic/CollisionManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/System/GameSystemManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/CharacterBase.h"
#include "../Object/Collider/ColliderFactory.h"
#include "../Object/Actor/Stage/TestModel.h"
#include "../Core/Game/Report/ReportSystem.h"
#include "ScenePause.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);
}

SceneGame::~SceneGame(void)
{
	//インスタンスの削除
	StageManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	ColliderFactory::GetInstance().Destroy();
	GameSystemManager::GetInstance().Destroy();
}

void SceneGame::Load(void)
{
	// 各種状態別処理の登録
	RegisterStateFunction(STATE::PLAY, std::bind(&SceneGame::UpdatePlay, this), std::bind(&SceneGame::DrawPlay, this));
	RegisterStateFunction(STATE::REPORTING, std::bind(&SceneGame::UpdateReporting, this), std::bind(&SceneGame::DrawReporting, this));

	// 親クラスの読み込み
	SceneBase::Load();	

	// コライダー生成管理クラス
	ColliderFactory::CreateInstance();
	
	// 衝突判定管理クラス
	CollisionManager::CreateInstance();

	// キャラクター
	CharacterManager::CreateInstance();
	CharacterManager::GetInstance().Load();

	// ステージ
	StageManager::CreateInstance();
	StageManager::GetInstance().Load();

	// システム
	GameSystemManager::CreateInstance();
	GameSystemManager::GetInstance().Load();

	//ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

	test_ = std::make_unique<TestModel>();
	test_->Load();
}

void SceneGame::Init(void)
{
	// キャラクター管理クラス初期化
	CharacterManager::GetInstance().Init();

	// ステージ管理クラス初期化
	StageManager::GetInstance().Init();

	// システム管理クラス初期化
	GameSystemManager::GetInstance().Init();

	test_->Init();

	// カメラ設定
	mainCamera.SetFollow(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FPS);
}

void SceneGame::NormalUpdate(void)
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	// 状態別更新処理
	stateUpdateMap_[state_]();

#ifdef _DEBUG	
	DebugUpdate();
#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG
	DebugDraw();
#endif
	
	// 状態別描画処理   
	stateDrawMap_[state_]();
}

void SceneGame::ChangeNormal(void)
{
	// 処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);
}

void SceneGame::RegisterStateFunction(const STATE state, std::function<void()> update, std::function<void()> draw)
{
	stateUpdateMap_[state] = update;
	stateDrawMap_[state] = draw;
}

void SceneGame::UpdatePlay()
{
	// キャラクターの本体更新
	CharacterManager::GetInstance().Update();

	// ステージ更新
	StageManager::GetInstance().Update();

	// システム更新
	GameSystemManager::GetInstance().Update();

	// 衝突判定の更新
	CollisionManager::GetInstance().Update();

	// コライダーの削除
	CollisionManager::GetInstance().Sweep();
}

void SceneGame::UpdateReporting()
{
	// 報告中の処理
	GameSystemManager::GetInstance().GetGamsSystem(GameSystemManager::TYPE::REPORTING).Update();
}

void SceneGame::DrawPlay()
{
	// ステージ描画
	StageManager::GetInstance().Draw();

	// キャラクター描画
	CharacterManager::GetInstance().Draw();

	// システム描画
	GameSystemManager::GetInstance().Draw();
}

void SceneGame::DrawReporting()
{
	// 報告中の描画
	GameSystemManager::GetInstance().GetGamsSystem(GameSystemManager::TYPE::REPORTING).Draw();
}

void SceneGame::DebugUpdate(void)
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		return;
	}

	// カメラモードの変更
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_CAMERA_CHANGE))
	{
		switch (mainCamera.GetMode())
		{
		case Camera::MODE::FPS:
			mainCamera.ChangeMode(Camera::MODE::FOLLOW);
			break;
		case Camera::MODE::FOLLOW:
			mainCamera.ChangeMode(Camera::MODE::FREE);
			break;
		case Camera::MODE::FREE:
			mainCamera.ChangeMode(Camera::MODE::FPS);
			break;
		default:
			break;
		};
	}

	test_->Update();
}

void SceneGame::DebugDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::CYAN,
		true
	);

	test_->Draw();
}