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
#include "../Object/Character/CharacterBase.h"
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
}

void SceneGame::Load(void)
{
	// 親クラスの読み込み
	SceneBase::Load();	
	
	// 衝突判定管理クラス
	CollisionManager::CreateInstance();

	// キャラクター
	CharacterManager::CreateInstance();
	CharacterManager::GetInstance().Load();

	// ステージ
	StageManager::CreateInstance();
	StageManager::GetInstance().Load();

	//ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();
}

void SceneGame::Init(void)
{
	// キャラクター管理クラス初期化
	CharacterManager::GetInstance().Init();

	// ステージ管理クラス初期化
	StageManager::GetInstance().Init();

	// カメラ設定
	mainCamera.SetFollow(&CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FPS);
}

void SceneGame::NormalUpdate(void)
{
	// ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	// キャラクター更新
	CharacterManager::GetInstance().Update();

	// ステージ更新
	StageManager::GetInstance().Update();

	// 衝突判定の更新
	CollisionManager::GetInstance().Update();

	// コライダーの削除
	CollisionManager::GetInstance().Sweep();
#ifdef _DEBUG
	DebugUpdate();
#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG
	DebugDraw();
#endif
	
	// ステージ描画
	StageManager::GetInstance().Draw();

	// キャラクター描画
	CharacterManager::GetInstance().Draw();
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
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RSHIFT))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// カメラモードの変更
	if (ins.IsTrgDown(KEY_INPUT_TAB))
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
