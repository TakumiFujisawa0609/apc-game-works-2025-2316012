#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../../Manager/Game/GameStateManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "GameStateEnemyRespown.h"

GameStateEnemyRespown::GameStateEnemyRespown()
{
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
	update_();
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
	ChangeState(STATE::END);
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