#include "SceneManager.h"
#include "../../Scene/State/Game/GameStateBase.h"
#include "../../Scene/State/Game/GameStateJumpScare.h"
#include "../../Scene/State/Game/GameStatePlay.h"
#include "../../Scene/State/Game/GameStateReporting.h"
#include "GameStateManager.h"

void GameStateManager::Load()
{
	// プレイ状態処理の生成
	auto play = std::make_unique<GameStatePlay>();
	stateMap_.emplace(STATE::PLAY, std::move(play));

	// 報告状態処理の生成
	auto repo = std::make_unique<GameStateReporting>();
	stateMap_.emplace(STATE::REPORTING, std::move(repo));

	// ジャンプスケア中の処理の生成
	auto jump = std::make_unique<GameStateJumpScare>();
	stateMap_.emplace(STATE::JUMP_SCARE, std::move(jump));
}

void GameStateManager::Init()
{
	// 状態別処理初期化
	for (auto& state : stateMap_)
	{
		state.second->Init();
	}

	// 初期状態
	state_ = STATE::PLAY;
}

void GameStateManager::Update()
{
	// ステートがNONEの場合
	if (state_ == STATE::NONE)
	{
		return;
	}

	stateMap_[state_]->Update();
}

void GameStateManager::Draw()
{
	if (state_ == STATE::NONE)
	{
		return;
	}

	stateMap_[state_]->Draw();
}

void GameStateManager::ChangeState(const STATE state)
{
	state_ = state;
}

GameStateManager::GameStateManager()
{
	state_ = STATE::NONE;
}

GameStateManager::~GameStateManager()
{
}
