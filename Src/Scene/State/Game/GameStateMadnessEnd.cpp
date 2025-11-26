#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "GameStateMadnessEnd.h"

GameStateMadnessEnd::GameStateMadnessEnd()
{
}

GameStateMadnessEnd::~GameStateMadnessEnd()
{
}

void GameStateMadnessEnd::Init()
{
}

void GameStateMadnessEnd::Update()
{
	// キャラクターの本体更新
	charaMng_.Update();

	// ステージ更新
	stageMng_.Update();
}

void GameStateMadnessEnd::Draw()
{
	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();                                                          
}
