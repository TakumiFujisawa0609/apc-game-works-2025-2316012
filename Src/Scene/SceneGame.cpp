#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Manager/Game/StageManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/GameStateManager.h"
#include "../Manager/Game/GameSystemManager.h"
#include "../Manager/Game/AnomalyManager.h"
#include "../Manager/Game/GameEffectManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Actor/Character/CharacterBase.h"
#include "../Object/Actor/Stage/StageObjectBase.h"
#include "../Object/Actor/Stage/TestModel.h"
#include "../Object/Collider/ColliderFactory.h"
#include "../Core/Game/ReportSystem.h"
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
	GameEffectManager::CreateInstance();
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
	GameEffectManager::GetInstance().Destroy();
}

void SceneGame::Load()
{
	// 親クラスの読み込み
	SceneBase::Load();	
}

void SceneGame::Init()
{
	// 基底クラスの初期化
	SceneBase::Init();

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

	// スコアを初期化
	ScoreManager::GetInstance().Init();

	// エフェクト管理クラスの初期化
	GameEffectManager::GetInstance().Init();

	// カメラ設定
	mainCamera.SetFollow(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FPS);

#ifdef _DEBUG	

	test_->Init();

#endif 

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::GAME);
}

void SceneGame::NormalUpdate()
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	// 状態別更新
	GameStateManager::GetInstance().Update();

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::NormalDraw()
{	
	// 状態別描画
	GameStateManager::GetInstance().Draw();

#ifdef _DEBUG

	DebugDraw();

#endif
}

void SceneGame::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}

void SceneGame::DebugUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::RESULT);
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

void SceneGame::DebugDraw()
{
	constexpr int INIT_POS_Y = 60;
	constexpr int OFFSET_Y = 20;
	int posY = INIT_POS_Y;

	createPositionList_->Draw();

	CollisionManager::GetInstance().DebugDraw();
	
	// カメラ位置
	VECTOR cPos = mainCamera.GetPos();
	VECTOR cTarget = mainCamera.GetTargetPos();
	VECTOR cAngles = mainCamera.GetAngles();

	// プレイヤー情報の取得
	auto& player = CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER);
	VECTOR playerPos = player.GetTransform().pos;
	float movePow = player.GetMoveSpeed();

	// 描画
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ角度：%2f,%2f,%2f", cAngles.x, cAngles.y, cAngles.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"プレイヤー位置：%2f,%2f,%2f", playerPos.x, playerPos.y, playerPos.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"移動量：%2f", movePow);
	posY += OFFSET_Y;

	AnomalyManager::GetInstance().DebugDraw();
}