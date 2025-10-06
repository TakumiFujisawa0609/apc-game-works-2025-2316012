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
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/CharacterBase.h"
#include "../Object/Collider/ColliderFactory.h"
#include "../Object/Actor/Stage/TestModel.h"
#include "State/Game/GameStateBase.h"
#include "State/Game/GameStatePlay.h"
#include "State/Game/GameStateReporting.h"
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
}

void SceneGame::Load(void)
{
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

	// プレイ状態別処理の登録
	auto statePlay = std::make_unique<GameStatePlay>();
	stateMap_.emplace(STATE::PLAY, statePlay);

	// 報告中状態別処理の登録
	auto stateRepo = std::make_unique<GameStateReporting>();
	stateMap_.emplace(STATE::REPORTING, stateRepo);

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

	// 状態別初期化処理
	for (const auto& state : stateMap_)
	{
		state.second->Init();
	}

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
	stateMap_[state_]->Update();

#ifdef _DEBUG	
	
	DebugUpdate();

	test_->Update();
#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG
	DebugDraw();

	test_->Draw();
#endif
	
	// 状態別描画処理
	stateMap_[state_]->Update();
}

void SceneGame::ChangeNormal(void)
{
	// 処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);
}

void SceneGame::DebugUpdate(void)
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_CAMERA_CHANGE))
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
}
