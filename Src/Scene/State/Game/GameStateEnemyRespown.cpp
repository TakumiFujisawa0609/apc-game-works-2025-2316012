#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../../Manager/Game/GameStateManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Object/Actor/Character/Enemy.h"
#include "GameStateEnemyRespown.h"

GameStateEnemyRespown::GameStateEnemyRespown()
{
	state_ = STATE::NONE;
	target_ = nullptr;

	// 状態変更関数管理
	changeStateMap_.emplace(STATE::NONE, std::bind(&GameStateEnemyRespown::UpdateNone, this));
	changeStateMap_.emplace(STATE::START, std::bind(&GameStateEnemyRespown::ChangeStateStart, this));
	changeStateMap_.emplace(STATE::WALK, std::bind(&GameStateEnemyRespown::ChangeStateWalk, this));
	changeStateMap_.emplace(STATE::MOVE_CAMERA_FORWARD, std::bind(&GameStateEnemyRespown::ChangeStateMoveCameraForward, this));
	changeStateMap_.emplace(STATE::ZOOM_OUT, std::bind(&GameStateEnemyRespown::ChangeStateZoomOut, this));
	changeStateMap_.emplace(STATE::ROAR, std::bind(&GameStateEnemyRespown::ChangeStateRoar, this));
	changeStateMap_.emplace(STATE::END, std::bind(&GameStateEnemyRespown::ChangeStateEnd, this));
}

GameStateEnemyRespown::~GameStateEnemyRespown()
{
}

void GameStateEnemyRespown::Init()
{
	ChangeState(STATE::START);
}

void GameStateEnemyRespown::Update()
{
	// 状態別更新
	update_();

	// ターゲットの更新
	if (target_) { target_->Update(); }
}

void GameStateEnemyRespown::Draw()
{
	// 影描画
	shadowMng_.Draw();

	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();

	// ポストエフェクトの描画
	effectMng_.Draw();
}

void GameStateEnemyRespown::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state_]();
}

void GameStateEnemyRespown::ChangeStateNone()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateNone, this);
}

void GameStateEnemyRespown::ChangeStateStart()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateStart, this);

	// プレイヤーの活動状態を非表示
	charaMng_.SetIsActive(CharacterManager::TYPE::PLAYER, false);

	// 敵のリストを取得
	auto& enemies = charaMng_.GetCharacters(CharacterManager::TYPE::ENEMY);

	// カメラ位置
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
	mainCamera.SetPos({ -1340, 52, -620 });
	mainCamera.SetTargetPos({ -1430, 30, -802 });

	// リストが空でない場合
	if (!enemies.empty()) 
	{
		// 先頭のポインタを取得してキャストする
		target_ = dynamic_cast<Enemy*>(enemies.front().get());
	}
	else 
	{
		// ターゲットを空にする
		target_ = nullptr;
	}

	// キャストが失敗した場合、アサートを発動する
	assert(target_ != nullptr && "dynamic_castに失敗しました");
}

void GameStateEnemyRespown::ChangeStateWalk()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateWalk, this);
}

void GameStateEnemyRespown::ChangeStateMoveCameraForward()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateMoveCameraForward, this);
}

void GameStateEnemyRespown::ChangeStateZoomOut()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateZoomOut, this);
}

void GameStateEnemyRespown::ChangeStateRoar()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateRoar, this);
}

void GameStateEnemyRespown::ChangeStateEnd()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateEnd, this);

	// 状態遷移
	GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::PLAY);

	// メッセージ表示
	systemMng_.ChangeMessage(Message::TYPE::ENEMY_RESPOWN);
}

void GameStateEnemyRespown::UpdateStart()
{
}

void GameStateEnemyRespown::UpdateWalk()
{
}

void GameStateEnemyRespown::UpdateMoveCameraForward()
{
}

void GameStateEnemyRespown::UpdateZoomOut()
{
}

void GameStateEnemyRespown::UpdateRoar()
{
}

void GameStateEnemyRespown::UpdateEnd()
{
}