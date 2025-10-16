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
#include "../Manager/System/AnomalyManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/CharacterBase.h"
#include "../Object/Collider/ColliderFactory.h"
#include "../Object/Actor/Stage/TestModel.h"
#include "../Core/Game/Report/ReportSystem.h"
#include "../Tool/CreatePositionList.h"
#include "State/Game/GameStateBase.h"
#include "State/Game/GameStatePlay.h"
#include "State/Game/GameStateReporting.h"
#include "ScenePause.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);
	// 各種変数の初期化
	state_ = STATE::MAX;
}

SceneGame::~SceneGame(void)
{
	//インスタンスの削除
	StageManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	ColliderFactory::GetInstance().Destroy();
	GameSystemManager::GetInstance().Destroy();
	AnomalyManager::GetInstance().Destroy();
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

	// システム
	GameSystemManager::CreateInstance();
	GameSystemManager::GetInstance().Load();

	//異常管理クラス
	AnomalyManager::CreateInstance();
	AnomalyManager::GetInstance().Load();

	//ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

	//プレイ状態処理の生成
	auto play = std::make_unique<GameStatePlay>(*this);
	stateMap_.emplace(STATE::PLAY, std::move(play));

	//報告状態処理の生成
	auto repo = std::make_unique<GameStateReporting>(*this);
	stateMap_.emplace(STATE::PLAY, std::move(repo));

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

	// 状態別処理初期化
	for (auto& state : stateMap_)
	{
		state.second->Init();
	}

#ifdef _DEBUG	
	test_->Init();
#endif 

	// カメラ設定
	mainCamera.SetFollow(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FPS);

	// 初期状態
	state_ = STATE::PLAY;
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
	
	// 状態別描画処理   
	stateMap_[state_]->Draw();

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
	
	// プレイヤー位置を取得
	VECTOR playerPos = CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform().pos;

	// 画面中心から座標を取得
	VECTOR screenCenter = ConvScreenPosToWorldPos({ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y, 0 });

	// 末端の位置を取得
	VECTOR endPos = VAdd(screenCenter, VScale(mainCamera.GetForward(), 1000.0f));

	// 先端位置を取得
	VECTOR startPos = playerPos;
	startPos.y += 120;


	// 描画
	DrawLine3D(startPos, endPos, UtilityCommon::BLUE);
	DrawCapsule3D(startPos, endPos, 30.0f, 10.0f, UtilityCommon::BLUE, UtilityCommon::BLUE, true);
	DrawSphere3D(startPos, 10.0f, 10.0f, UtilityCommon::GREEN, UtilityCommon::GREEN, true);
	DrawSphere3D(endPos, 10.0f, 10.0f, UtilityCommon::LIME, UtilityCommon::LIME, true);
	DrawSphere3D(VGet(0.0f, 0.0f, 50.0f), 20.0f, 10.0f, UtilityCommon::RED, UtilityCommon::RED, true);

	// カメラ関係の値
	DrawFormatString(0, 60, UtilityCommon::RED, L"始点の位置：%2f,%2f,%2f", startPos.x, startPos.y, startPos.z);
	DrawFormatString(0, 80, UtilityCommon::RED, L"末端の位置：%2f,%2f,%2f", endPos.x, endPos.y, endPos.z);
}