#include "../../Scene/State/Game/GameStateBase.h"
#include "../../Scene/State/Game/GameStateJumpScare.h"
#include "../../Scene/State/Game/GameStatePlay.h"
#include "../../Scene/State/Game/GameStateReporting.h"
#include "../../Scene/State/Game/GameStateMadnessEnd.h"
#include "../Resource/SoundManager.h"
#include "../System/ScoreManager.h"
#include "SceneManager.h"
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

	// 狂気終了処理の生成
	auto madnessEnd = std::make_unique<GameStateMadnessEnd>();
	stateMap_.emplace(STATE::MADNESS_END, std::move(madnessEnd));
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

void GameStateManager::SetGameClear()
{
	// ゲームの終了状態を設定
	ScoreManager::GetInstance().SetEndState(ScoreManager::END_STATE::CLEAR);

	// シーンをリザルトへ
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);

	// 自身の状態をNONEにする
	state_ = STATE::NONE;

	// サウンドの停止
	SoundManager::GetInstance().StopAllSe();	
	SoundManager::GetInstance().StopBgm(SoundType::BGM::GAME);	
}

void GameStateManager::SetGameOver()
{
	// ゲームの終了状態を設定
	ScoreManager::GetInstance().SetEndState(ScoreManager::END_STATE::DEAD);

	// シーンをリザルトへ
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);

	// 自身の状態をNONEにする
	state_ = STATE::NONE;

	// サウンドの停止
	SoundManager::GetInstance().StopAllSe();
	SoundManager::GetInstance().StopBgm(SoundType::BGM::GAME);
}

GameStateManager::GameStateManager()
{
	state_ = STATE::NONE;
}

GameStateManager::~GameStateManager()
{
}