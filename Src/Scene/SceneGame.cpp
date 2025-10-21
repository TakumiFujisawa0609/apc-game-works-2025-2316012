#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/StageManager.h"
#include "../Manager/Generic/CharacterManager.h"
#include "../Manager/Generic/CollisionManager.h"
#include "../Manager/Generic/GameStateManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/System/GameSystemManager.h"
#include "../Manager/System/AnomalyManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/CharacterBase.h"
#include "../Object/Collider/ColliderFactory.h"
#include "../Object/Actor/Stage/TestModel.h"
#include "../Core/Game/Report/ReportSystem.h"
#include "../Tool/CreatePositionList.h"
#include "State/Game/GameStateBase.h"
#include "ScenePause.h"
#include "SceneGame.h"

SceneGame::SceneGame()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);

	// インスタンスの生成
	ColliderFactory::CreateInstance();
	CollisionManager::CreateInstance();
	CharacterManager::CreateInstance();
	StageManager::CreateInstance();
	GameSystemManager::CreateInstance();
	AnomalyManager::CreateInstance();
	GameStateManager::CreateInstance();
}

SceneGame::~SceneGame()
{
	// インスタンスの削除
	StageManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	ColliderFactory::GetInstance().Destroy();
	GameSystemManager::GetInstance().Destroy();
	AnomalyManager::GetInstance().Destroy();
	GameStateManager::GetInstance().Destroy();
}

void SceneGame::Load()
{
	// 親クラスの読み込み
	SceneBase::Load();	

	// キャラクター
	CharacterManager::GetInstance().Load();

	// ステージ
	StageManager::GetInstance().Load();

	// システム
	GameSystemManager::GetInstance().Load();

	// 異常
	AnomalyManager::GetInstance().Load();

	// ゲーム状態管理
	GameStateManager::GetInstance().Load();

	// ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

#ifdef _DEBUG	
	// テスト用モデル
	test_ = std::make_unique<TestModel>();
	test_->Load();

	// テスト用の座標リスト作成クラス
	createPositionList_ = std::make_unique<CreatePositionList>("EnemyPosList", CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
#endif 
}

void SceneGame::Init(void)
{
	// キャラクター管理クラス初期化
	CharacterManager::GetInstance().Init();

	// ステージ管理クラス初期化
	StageManager::GetInstance().Init();

	// システム管理クラス初期化
	GameSystemManager::GetInstance().Init();

	// 異変管理クラス初期化
	AnomalyManager::GetInstance().Init();

	// ゲーム状態管理クラス初期化
	GameStateManager::GetInstance().Init();

	// カメラ設定
	mainCamera.SetFollow(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FPS);

#ifdef _DEBUG	
	test_->Init();
#endif 
}

void SceneGame::NormalUpdate(void)
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	GameStateManager::GetInstance().Update();

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::CYAN,
		true
	);
#endif
	
	GameStateManager::GetInstance().Draw();

#ifdef _DEBUG

	DebugDraw();

#endif
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

	createPositionList_->Update();
}

void SceneGame::DebugDraw(void)
{
	test_->Draw();
	createPositionList_->Draw();

	CollisionManager::GetInstance().DebugDraw();
	
	// カメラ位置
	VECTOR cPos = mainCamera.GetPos();
	VECTOR cTarget = mainCamera.GetTargetPos();

	// 描画
	DrawFormatString(0, 60, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	DrawFormatString(0, 80, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
}